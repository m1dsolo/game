#pragma once

#include <core/component/range_attack.hpp>
#include <core/component/reload.hpp>

#include <string>
#include <vector>

namespace core {

struct Item {
    enum class Rarity {
        common,
        uncommon,
        rare,
        epic,
        legendary,
    };

    enum class Type {
        weapon,
        equipment,
    };

    struct Components {
        std::optional<RangeAttackComponent> range_attack;
        std::optional<ReloadComponent> reload;
        std::optional<float> range;
    };
};

struct ItemConfig {
    std::string name;
    std::optional<std::string> sprite;
    Item::Rarity rarity;
    Item::Type type;
    int weight;
    std::vector<std::string> description;
    Item::Components components;

    wheel::ID sprite_id;
};

}  // namespace core
