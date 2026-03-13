# ModelResharder-Transformers C++

A robust C++ CLI tool for downloading, resharding, and re-uploading large Hugging Face models. 
It uses `pybind11` to seamlessly bind with the Python Hugging Face ecosystem (`transformers`, `huggingface_hub`, `torch`), offering the safety and structure of C++ with the vast library support of Python.

## Prerequisites

- **C++17 Compiler** (GCC, Clang, MSVC)
- **CMake** (v3.14+)
- **Python 3.10+** (must be accessible to CMake)
- Python packages: `torch`, `transformers`, `huggingface_hub`

## Build Instructions

You can build the project from the root of the repository using CMake:

1. **Configure the Project**
   ```bash
   cmake -B build -S .
   ```

2. **Build the Project**
   ```bash
   cmake --build build --config Release
   ```

## Usage

After building, you can run the compiled executable located in the `build/Release` directory (or `build/` on Linux/macOS) with the required arguments.

### Example: Resharding Qwen3.5

**Windows (PowerShell / CMD):**
```powershell
.\build\Release\resharder.exe --model_name username/source-model_page `
                              --repo_id your-username/target-model_page `
                              --token <YOUR_HF_TOKEN> `
                              --shard_size 4.4GB `
                              --arch Qwen3_5ForConditionalGeneration
```

**Linux / macOS (Bash):**
```bash
./build/resharder --model_name username/source-model_page \
                  --repo_id your-username/target-model_page \
                  --token <YOUR_HF_TOKEN> \
                  --shard_size 4.4GB \
                  --arch Qwen3_5ForConditionalGeneration
```

### Options
- `--model_name`: Original HuggingFace Model Name
- `--repo_id`: Target HuggingFace Repository ID (will be created as private)
- `--token`: Your HuggingFace Write Token (ensure you keep this secure and don't commit it!)
- `--shard_size`: Max Shard Size (default: `4.4GB`)
- `--arch`: Model Architecture Class Name (default: `Qwen3VLForConditionalGeneration`, make sure to pass the right architecture for your model, e.g., `Qwen2ForCausalLM` or `Qwen3_5ForConditionalGeneration`)

### Important Note on Security
**Never expose or commit your HuggingFace token (`hf_...`).** Pass it securely via the command line when executing the binary.
