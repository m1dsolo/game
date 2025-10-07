#pragma once

#include <ecs/entity.hpp>

union SDL_Event;

namespace core {

class UI {
public:
    static bool handle_layout_event(wheel::Entity entity, const SDL_Event& event);
};

}  // namespace core
