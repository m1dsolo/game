#pragma once

#include <sdl/sdl.hpp>

namespace core {

struct ContextResource {
    bool running = true;
    SDL_Texture* texture = nullptr;
    SDL_FRect viewport{0, 0, 0, 0};
};

}  // namespace core
