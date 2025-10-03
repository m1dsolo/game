#pragma once

#include <string>
#include <vector>

namespace core {

struct InventoryComponent {
    InventoryComponent(int size) {
        items.resize(size);
    }
    std::vector<std::pair<std::string, int>> items;
};

}  // namespace core
