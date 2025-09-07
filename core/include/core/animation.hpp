#pragma once

#include <core/sprite.hpp>

#include <vector>

namespace core {

struct Animation {
    std::string name;  // "slime-idle"
    std::vector<Sprite> sprites;
    float duration = 1.f;
    bool loop = true;
};

}  // namespace core
