#pragma once

#include <ecs/entity.hpp>

namespace core {

struct ProjectileComponent {
    int damage = 10;
    wheel::Entity last;
};

}  // namespace core
