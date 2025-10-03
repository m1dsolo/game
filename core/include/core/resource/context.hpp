#pragma once

#include <sdl/sdl.hpp>

namespace core {

struct ContextResource {
    bool running = true;
    SDL_Texture* texture = nullptr;
    SDL_FRect viewport{0, 0, 0, 0};

    int fps = 60;
    int virtual_window_width = 1920;
    int virtual_window_height = 1080;
    int map_width = 1200;
    int map_height = 900;
};

}  // namespace core
