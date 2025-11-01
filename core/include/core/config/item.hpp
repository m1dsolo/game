#pragma once

#include <core/component/range_attack.hpp>

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
    };
};

struct ItemConfig {
    std::string name;
    Item::Rarity rarity;
    Item::Type type;
    std::vector<std::string> description;
    Item::Components components;
};

}  // namespace core
