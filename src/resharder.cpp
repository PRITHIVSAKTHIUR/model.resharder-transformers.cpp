#include "resharder.hpp"
#include <iostream>
#include <pybind11/embed.h>

namespace py = pybind11;
using namespace pybind11::literals;

namespace resharder {

bool load_and_reshard(
    const std::string& model_name,
    const std::string& new_repo_id,
    const std::string& hf_token,
    const std::string& max_shard_size,
    const std::string& architecture
) {
    try {
        py::print("Initializing Python environment...");

        // Import necessary Python modules
        py::module_ torch = py::module_::import("torch");
        py::module_ transformers = py::module_::import("transformers");
        py::module_ hf_hub = py::module_::import("huggingface_hub");
        py::module_ os = py::module_::import("os");
        py::module_ shutil = py::module_::import("shutil");

        // Login to Hugging Face
        py::print("Logging into Hugging Face...");
        hf_hub.attr("login")("token"_a=hf_token);

        // Create Repo
        py::print("Creating/Checking repository: ", new_repo_id);
        hf_hub.attr("create_repo")(
            "repo_id"_a=new_repo_id,
            "private"_a=true,
            "exist_ok"_a=true
        );

        // Load Processor
        py::print("Loading processor for: ", model_name);
        py::object processor = transformers.attr("AutoProcessor").attr("from_pretrained")(
            model_name,
            "trust_remote_code"_a=true
        );

        // Load Model
        py::print("Loading model: ", model_name, " with architecture: ", architecture);
        py::object ModelClass;
        if (architecture == "Qwen3_5ForConditionalGeneration" || 
            architecture == "Qwen3VLForConditionalGeneration" ||
            architecture == "Qwen2_5_VLForConditionalGeneration" ||
            architecture == "Qwen2VLForConditionalGeneration") {
            ModelClass = transformers.attr(architecture.c_str());
        } else {
            ModelClass = transformers.attr("AutoModelForCausalLM"); // Fallback
        }

        py::object model = ModelClass.attr("from_pretrained")(
            model_name,
            "torch_dtype"_a="auto",
            "device_map"_a="auto",
            "trust_remote_code"_a=true,
            "use_safetensors"_a=true
        );
        model.attr("eval")();

        // Local directory
        std::string local_dir = "_resharded_cpp_tmp";
        os.attr("makedirs")(local_dir, "exist_ok"_a=true);

        // Save
        py::print("Saving model shards with max size: ", max_shard_size);
        model.attr("save_pretrained")(
            local_dir,
            "max_shard_size"_a=max_shard_size
        );
        processor.attr("save_pretrained")(local_dir);

        py::print("Model and processor saved locally.");

        // Clean up memory
        model.release();
        py::module_ gc = py::module_::import("gc");
        gc.attr("collect")();
        py::object cuda = torch.attr("cuda");
        if (cuda.attr("is_available")().cast<bool>()) {
            cuda.attr("empty_cache")();
        }

        // Upload
        py::print("Uploading model to Hugging Face Hub...");
        hf_hub.attr("upload_large_folder")(
            "repo_id"_a=new_repo_id,
            "repo_type"_a="model",
            "folder_path"_a=local_dir,
            "revision"_a="main"
        );

        py::print("Upload complete! Model live at https://huggingface.co/", new_repo_id);

        // Cleanup
        shutil.attr("rmtree")(local_dir);
        return true;
    } catch (const py::error_already_set& e) {
        std::cerr << "Python error occurred: " << e.what() << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cerr << "C++ error occurred: " << e.what() << std::endl;
        return false;
    }
}

} // namespace resharder
