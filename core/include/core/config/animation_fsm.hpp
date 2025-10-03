#pragma once

#include <string>
#include <vector>
#include <variant>

namespace core {

struct TransitionConfig {
    std::variant<std::string, std::vector<std::string>> from;
    std::variant<std::string, std::vector<std::string>> to;
    std::variant<std::string, std::vector<std::string>> conditions;
};

struct AnimationFSMConfig {
    std::string name;
    std::vector<TransitionConfig> transitions;
};

}  // namespace core
