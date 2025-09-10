#include <core/util/util.hpp>

#include <wheel/random.hpp>

namespace core {

wheel::Vector2D<float> Util::random_pick_point(const wheel::Rect<float>& rect) {
    auto& random = wheel::Random::instance();

    return {
        random.uniform(rect.x0, rect.x1),
        random.uniform(rect.y0, rect.y1)
    };
}

}  // namespace core
