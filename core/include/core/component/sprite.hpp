#pragma once

#include <sdl/sdl.hpp>

namespace core {

struct Sprite {
    Sprite() {}
    Sprite(
        SDL_Texture* texture,
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

struct SpriteComponent {
    SpriteComponent(const std::string& name = "");
    SpriteComponent(SDL_FColor color);

    const Sprite* sprite;
    bool flip = false;
    SDL_FColor color = sdl::SDL::Color::White;
    int get_hit_effect_cnt = 0;
};

}  // namespace core
