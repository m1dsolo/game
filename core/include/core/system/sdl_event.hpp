#pragma once

#include <sdl/sdl.hpp>

namespace core {

struct SDLEventSystem {
    void operator()();

private:
    SDL_Event event_;
};

}  // namespace core
