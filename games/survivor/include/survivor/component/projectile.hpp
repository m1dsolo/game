#pragma once

#include <ecs/entity.hpp>

namespace survivor {

struct ProjectileComponent {
    int damage = 10;
    wheel::Entity last;
};

}  // namespace survivor
