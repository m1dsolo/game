#pragma once

#include <wheel/ecs.hpp>

namespace wheel {

struct CollideComponent {
    int atk;
    int durability = -1;  // -1 means infinite durability
    int cooldown = 0;  // 0 means no cooldown
};

struct UniqueCollideComponent {
    Entity last = EntityNone;
};

}  // namespace wheel
