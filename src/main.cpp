#include <iostream>
#include <string>
#include <pybind11/embed.h>
#include "resharder.hpp"
#include "utils.hpp"

namespace py = pybind11;

int main(int argc, char* argv[]) {
    utils::print_banner();

    std::string model_name = "";
    std::string repo_id = "";
    std::string token = "";
    std::string shard_size = "4.4GB";
    std::string architecture = "Qwen3VLForConditionalGeneration";

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "--help") {
            utils::print_help(argv[0]);
            return 0;
        } else if (arg == "--model_name" && i + 1 < argc) {
            model_name = argv[++i];
        } else if (arg == "--repo_id" && i + 1 < argc) {
            repo_id = argv[++i];
        } else if (arg == "--token" && i + 1 < argc) {
            token = argv[++i];
        } else if (arg == "--shard_size" && i + 1 < argc) {
            shard_size = argv[++i];
        } else if (arg == "--arch" && i + 1 < argc) {
            architecture = argv[++i];
        }
    }

    if (model_name.empty() || repo_id.empty() || token.empty()) {
        std::cerr << "Error: Missing required arguments.\n\n";
        utils::print_help(argv[0]);
        return 1;
    }

    std::cout << "Model Name: " << model_name << "\n";
    std::cout << "Repository ID: " << repo_id << "\n";
    std::cout << "Shard Size: " << shard_size << "\n";
    std::cout << "Architecture: " << architecture << "\n\n";

    try {
        // Initialize Python interpreter
        py::scoped_interpreter guard{}; 

        bool success = resharder::load_and_reshard(
            model_name, repo_id, token, shard_size, architecture
        );

        if (success) {
            std::cout << "\nOperation completed successfully!\n";
            return 0;
        } else {
            std::cerr << "\nOperation failed.\n";
            return 1;
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize embedded Python: " << e.what() << "\n";
        return 1;
    }
}