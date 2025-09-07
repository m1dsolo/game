#pragma once

#include <core/sprite.hpp>
#include <core/manager/sprite.hpp>

namespace core {

struct SpriteComponent {
    SpriteComponent(const std::string& name = "")
        : sprite(&SpriteManager::instance().get(name)) {}

    const Sprite* sprite;
};

}  // namespace core
