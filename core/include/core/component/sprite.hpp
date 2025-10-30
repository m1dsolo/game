#pragma once

#include <core/other/sprite.hpp>

#include <wheel/id.hpp>
#include <sdl/sdl.hpp>

namespace core {

struct SpriteComponent {
    SpriteComponent(wheel::ID id = "");

    wheel::ID id;
    const Sprite* sprite;

    bool flip = false;
    SDL_FColor color = sdl::SDL::Color::White;
    int get_hit_effect_cnt = 0;
};

}  // namespace core
