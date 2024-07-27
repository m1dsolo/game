#pragma once

#include <unordered_map>

#include <wheel/ecs.hpp>

#include <SDL3/SDL.h>

namespace wheel {

struct InputComponent {
    std::unordered_map<SDL_Keycode, std::string> key_bindings;
};

}  // namespace wheel
