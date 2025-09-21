#pragma once

#include <wheel/geometry.hpp>
#include <sdl/sdl.hpp>

namespace core {

struct Sprite {
    Sprite() {}
    Sprite(
        SDL_Texture* texture,
        SDL_FRect rect = {0.f, 0.f, 0.f, 0.f},
        SDL_FColor color = sdl::SDL::WHITE
    ) : texture(texture), rect(rect), color(color) {
        if (rect.w == 0.f && rect.h == 0.f) {
            auto [w, h] = sdl::SDL::get_texture_size(texture);
            this->rect.w = static_cast<float>(w);
            this->rect.h = static_cast<float>(h);
        }
    }

    SDL_Texture* texture = nullptr;
    SDL_FRect rect{0.f, 0.f, 0.f, 0.f};
    SDL_FColor color = sdl::SDL::WHITE;
};

}  // namespace core
