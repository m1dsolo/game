#pragma once

#include <vector>

#include <wheel/id.hpp>

namespace core {

struct Slot {
    wheel::ID item_id = "";
    int num = 0;
};

struct InventoryComponent {
    std::vector<Slot> hotbar;
    std::vector<Slot> backpack;
    int max_weapon_weight = 1;
    int max_equipment_weight = 1;

    int weapon_weight = 0;
    int equipment_weight = 0;

    int equip(int idx);
    int unequip(int idx);
    int pickup(wheel::ID item_id, int num);
    int combine(int idx);
};

}  // namespace core
