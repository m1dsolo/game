#pragma once

#include <core/component/loot.hpp>

#include <string>
#include <vector>

namespace core {

struct AttackConfig {
    int damage = 0;
    float range = 50.f;
    int interval = 1000000;
};

struct EnemyConfig {
    std::string name;
    int hp = 100;
    float speed = 100.f;
    AttackConfig attack;
    std::vector<LootGroup> loots;
};

}  // namespace core
