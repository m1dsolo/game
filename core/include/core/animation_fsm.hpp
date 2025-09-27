#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace core {

struct AnimationFSM {
    std::string name;
    // state -> [(next_state, [conditions])]
    std::unordered_map<std::string, std::vector<std::pair<std::string, std::vector<std::string>>>> transitions;
};

}  // namespace core
