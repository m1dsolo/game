#pragma once

#include <core/animation_fsm.hpp>

#include <string>
#include <unordered_map>

namespace core {

struct AnimationFSMComponent {
    AnimationFSMComponent(const std::string& key = "");

    const AnimationFSM* fsm;
    std::string current_state = "start";
    std::unordered_map<std::string, bool> next_condition2values;
    std::unordered_map<std::string, bool> current_condition2values;
};

}  // namespace core
