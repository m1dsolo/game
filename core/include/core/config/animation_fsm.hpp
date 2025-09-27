#pragma once

#include <string>
#include <vector>

namespace core {

struct TransitionConfig {
    std::string from;
    std::string to;
    std::vector<std::string> conditions;
};

struct AnimationFSMConfig {
    std::string name;
    std::vector<TransitionConfig> transitions;
};

}  // namespace core
