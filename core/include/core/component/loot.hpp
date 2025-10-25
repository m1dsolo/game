#pragma once

#include <core/config/enemy.hpp>

namespace core {

struct LootComponent {
    std::vector<LootGroupConfig> groups;
};

}  // namespace core
