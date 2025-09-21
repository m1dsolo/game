#pragma once

#include <core/sprite.hpp>

namespace core {

struct SpriteComponent {
    SpriteComponent(const std::string& name = "");
    SpriteComponent(SDL_FColor color);

    const Sprite* sprite;
};

}  // namespace core
