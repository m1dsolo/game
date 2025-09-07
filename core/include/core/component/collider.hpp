#pragma once

#include <wheel/geometry.hpp>

namespace core {

struct ColliderComponent {
    wheel::Vector2D<float> size;  // TODO: std::variant and add circle
    bool dynamic = true;
    bool trigger = false;  // not managed by ColliderManager
};

}  // namespace core
