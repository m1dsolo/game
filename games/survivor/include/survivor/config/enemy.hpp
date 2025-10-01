#pragma once

#include <survivor/component/loot.hpp>

#include <string>
#include <vector>

namespace survivor {

struct EnemyConfig {
    std::string name;
    int hp = 100;
    float speed = 100.f;
    std::vector<LootGroup> loots;
};

}  // namespace survivor
