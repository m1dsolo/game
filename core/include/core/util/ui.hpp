#pragma once

#include <ecs/entity.hpp>

#include <string_view>
#include <vector>

union SDL_Event;

namespace core {

class UI {
public:
    static wheel::Entity add_vertical_button_layout(
        std::string_view name,
        std::vector<std::string_view> texts,
        float button_width,
        float button_height,
        float spacing
    );
    static bool handle_layout_event(wheel::Entity entity, const SDL_Event& event);
};

}  // namespace core
