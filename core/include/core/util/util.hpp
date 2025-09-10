#pragma once

#include <wheel/geometry.hpp>

namespace core {

class Util {
public:
    static wheel::Vector2D<float> random_pick_point(const wheel::Rect<float>& rect);
};

}  // namespace core
