#include "utils.hpp"
#include <iostream>

namespace utils {

void print_banner() {
    std::cout << "===========================================\n";
    std::cout << "   ModelResharder-Transformers C++ CLI     \n";
    std::cout << "===========================================\n";
}

void print_help(const char* prog_name) {
    std::cout << "Usage: " << prog_name << " [OPTIONS]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --model_name <name>       Original HuggingFace Model Name\n";
    std::cout << "  --repo_id <id>            New HuggingFace Repository ID\n";
    std::cout << "  --token <token>           HuggingFace Write Token\n";
    std::cout << "  --shard_size <size>       Max Shard Size (e.g., 4.4GB)\n";
    std::cout << "  --arch <architecture>     Model Architecture Class Name\n";
    std::cout << "  --help                    Show this help message\n\n";
    std::cout << "Example:\n";
    std::cout << "  " << prog_name << " --model_name Qwen/Qwen3-VL-2B-Instruct --repo_id username/sharded-model --token hf_... --shard_size 2GB --arch Qwen3VLForConditionalGeneration\n";
}

}
