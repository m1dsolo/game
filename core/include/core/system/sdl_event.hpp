#pragma once

#include <core/system/base.hpp>

#include <sdl/sdl.hpp>

namespace core {

class SDLEventSystem : public BaseSystem {
public:
    SDLEventSystem() : BaseSystem("SDLEvent") {}

    void update_impl() override;

private:
    SDL_Event event_;
};

}  // namespace core
