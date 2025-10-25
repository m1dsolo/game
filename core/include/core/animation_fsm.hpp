#pragma once

#include <wheel/id.hpp>

#include <vector>
#include <unordered_map>

namespace core {

struct AnimationFSM {
    // animation fsm ID
    wheel::ID id;
    // state -> [(next_state, [conditions])]
    std::unordered_map<wheel::ID, std::vector<std::pair<wheel::ID, std::vector<wheel::ID>>>> transitions;
};

}  // namespace core
