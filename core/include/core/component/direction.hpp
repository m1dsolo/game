#pragma once

#include <wheel/geometry.hpp>

namespace core {

struct DirectionComponent {
    wheel::Vector2D<float> move{0.f, 0.f};
    wheel::Vector2D<float> look{0.f, 1.f};
};

}  // namespace core
