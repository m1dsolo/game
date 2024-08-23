#pragma once

#include <wheel/geometry.hpp>

namespace wheel {

struct PositionComponent {
    Vector2D<float> vec {0., 0.};
};

struct RelativePositionComponent {
    Vector2D<float> vec {0., 0.};
};

}  // namespace wheel
