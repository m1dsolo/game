#pragma once

#include <string>
#include <vector>
#include <core/config/item.hpp>

namespace core {

struct AttackConfig {
    int damage = 0;
    float range = 50.f;
    int interval = 1000000;
};

struct LootItemConfig {
    std::string name;
    Item::Rarity rarity = Item::Rarity::common;
    std::pair<int, int> count;
    int weight = 1;
};

struct LootGroupConfig {
    std::vector<LootItemConfig> items;
    float chance = 1.f;
};

struct EnemyConfig {
    std::string name;
    int hp = 100;
    float speed = 100.f;
    AttackConfig attack;
    std::vector<LootGroupConfig> loots;
};

}  // namespace core
