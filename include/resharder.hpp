#pragma once
#include <string>

namespace resharder {
    bool load_and_reshard(
        const std::string& model_name,
        const std::string& new_repo_id,
        const std::string& hf_token,
        const std::string& max_shard_size,
        const std::string& architecture
    );
}
