#pragma once

#include <vector>

#include <wheel/ecs.hpp>

namespace wheel {

struct DamageComponent {
    // damage, attacker entity
    std::vector<std::pair<int, Entity>> damages;
};

struct DeadComponent {
    Entity killer_entity;
};

}  // namespace wheel
