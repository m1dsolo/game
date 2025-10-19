#pragma once

#include <string>

namespace core {

enum class Rarity {
    common,
    uncommon,
    rare,
    epic,
    legendary
};

struct ItemConfig {
    std::string name;
    Rarity rarity;
};

}  // namespace core
