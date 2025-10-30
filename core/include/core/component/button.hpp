#pragma once

#include <wheel/id.hpp>

namespace core {

struct ButtonComponent {
    wheel::ID normal_color_id = "button_normal";
    wheel::ID hovered_color_id = "button_hovered";
    wheel::ID pressed_color_id = "button_pressed";

    bool is_hovered = false;
};

}  // namespace core
