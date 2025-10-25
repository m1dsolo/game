#pragma once

#include <core/animation_fsm.hpp>

#include <unordered_map>

namespace core {

struct AnimationFSMComponent {
    AnimationFSMComponent(wheel::ID id);

    const AnimationFSM* fsm;
    wheel::ID current_state_id = "start";
    std::unordered_map<wheel::ID, bool> next_condition_id2values;
    std::unordered_map<wheel::ID, bool> current_condition_id2values;
};

}  // namespace core
