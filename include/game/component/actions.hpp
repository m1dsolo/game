#pragma once

#include <string>
#include <memory>

#include <game/action.hpp>

namespace wheel {

struct ActionsComponent {
    std::unordered_map<std::string, std::shared_ptr<Action>> action_map;
};

}  // namespace wheel
