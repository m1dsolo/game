#pragma once

#include <string>
#include <vector>

namespace core {

struct LootItem {
    std::string name;
    std::pair<int, int> count;
    int weight = 1;
};

struct LootGroup {
    std::vector<LootItem> items;
    float chance = 1.f;
};

struct LootComponent {
    std::vector<LootGroup> groups;
};

}  // namespace core
