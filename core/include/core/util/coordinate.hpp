#pragma once

#include <sdl/sdl.hpp>

namespace core {

class Coordinate {
public:
    static SDL_FRect world2screen(const SDL_FRect& world_rect);
};

}  // namespace core
