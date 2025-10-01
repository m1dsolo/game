#pragma once

#include <core/component/collider.hpp>

namespace survivor {

namespace ColliderLayer {

enum ColliderLayer : core::ColliderLayerType {
    Player = 1 << 0,
    Enemy = 1 << 1,
    Obstacle = 1 << 2,
    Projectile = 1 << 3,
    Trigger = 1 << 4,
    Item = 1 << 5,
};

}  // namespace ColliderLayer

}  // namespace survivor
