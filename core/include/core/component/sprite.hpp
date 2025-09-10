#pragma once

#include <core/sprite.hpp>

namespace core {

struct SpriteComponent {
    SpriteComponent(const std::string& name = "");

    const Sprite* sprite;
};

}  // namespace core
