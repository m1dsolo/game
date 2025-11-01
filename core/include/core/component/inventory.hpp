#pragma once

#include <vector>

#include <wheel/id.hpp>

namespace core {

struct Slot {
    wheel::ID item_id = "";
    int num = 0;
};

struct InventoryComponent {
    std::vector<Slot> slots;
};

}  // namespace core
