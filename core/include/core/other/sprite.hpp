#pragma once

#include <sdl/sdl.hpp>

namespace core {

struct Sprite {
    Sprite(
        SDL_Texture* texture = nullptr,
        SDL_FRect rect = {0.f, 0.f, 0.f, 0.f}
    ) : texture(texture), rect(rect) {
        if (rect.w == 0.f && rect.h == 0.f) {
            auto [w, h] = sdl::SDL::get_texture_size(texture);
            this->rect.w = static_cast<float>(w);
            this->rect.h = static_cast<float>(h);
        }
    }

    SDL_Texture* texture = nullptr;
    SDL_FRect rect{0.f, 0.f, 0.f, 0.f};
};

}  // namespace core
