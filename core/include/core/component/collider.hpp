#pragma once

#include <wheel/geometry.hpp>

#include <variant>

namespace core {

using ColliderLayerType = uint32_t;

struct ColliderComponent {
    std::variant<wheel::Rect<float>, wheel::Circle<float>> shape;
    ColliderLayerType layer = 0;
    ColliderLayerType mask = 0;

    wheel::Vector2D<float> size() const {
        return std::visit([](const auto& shape) {
            return shape.size();
        }, shape);
    }

    bool is_overlapping(const ColliderComponent& other) const {
        return std::visit([](const auto& shape1, const auto& shape2) {
            return shape1.is_overlapping(shape2);
        }, shape, other.shape);
    }
};

enum ColliderLayer : ColliderLayerType {
    Player = 1 << 0,
    Enemy = 1 << 1,
    Obstacle = 1 << 2,
    Projectile = 1 << 3,
    Trigger = 1 << 4,
    Item = 1 << 5,
};

}  // namespace core
