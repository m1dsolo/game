#pragma once

#include <core/manager/sprite.hpp>

#include <vector>

namespace core {

struct SpriteConfig {
    std::string name;
    std::pair<int, int> pos;
};

struct SpritesConfig {
    std::string path;
    std::pair<int, int> size;
    std::vector<SpriteConfig> sprites;
};

}  // namespace core
