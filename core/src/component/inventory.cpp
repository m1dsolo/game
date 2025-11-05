#include <core/component/inventory.hpp>
#include <core/global.hpp>
#include <core/manager/item.hpp>
#include <core/event/equip.hpp>

#include <iostream>

namespace core {

// TODO: optimize
int InventoryComponent::equip(int idx) {
    auto& slot = backpack[idx];
    auto& item_info = ItemManager::instance().get(slot.item_id);

    auto& weight = item_info.type == Item::Type::weapon ? weapon_weight : equipment_weight;
    if (weight + item_info.weight > max_weapon_weight) {
        return -1;
    }
    weight += item_info.weight;
    for (auto [i, hotbar_slot] : std::views::enumerate(hotbar)) {
        if (hotbar_slot.item_id == "") {
            hotbar_slot.item_id = slot.item_id;
            hotbar_slot.num = 1;
            slot.num -= 1;
            if (slot.num == 0) {
                slot.item_id = "";
            }
            ecs.emplace_event<EquipEvent>(hotbar_slot.item_id, i);
            return i;
        }
    }

    return -1;
}

// TODO: optimize
int InventoryComponent::unequip(int idx) {
    auto& hotbar_slot = hotbar[idx];
    auto& item_info = ItemManager::instance().get(hotbar_slot.item_id);

    for (auto [i, backpack_slot] : std::views::enumerate(backpack)) {
        if (backpack_slot.item_id == hotbar_slot.item_id) {
            backpack_slot.num += 1;
            hotbar_slot.item_id = "";
            hotbar_slot.num = 0;
            auto& weight = item_info.type == Item::Type::weapon ? weapon_weight : equipment_weight;
            weight -= item_info.weight;
            ecs.emplace_event<UnequipEvent>(hotbar_slot.item_id, idx);
            return i;
        }
    }

    for (auto [i, backpack_slot] : std::views::enumerate(backpack)) {
        if (backpack_slot.item_id == "") {
            backpack_slot.item_id = hotbar_slot.item_id;
            backpack_slot.num += 1;
            hotbar_slot.item_id = "";
            auto& weight = item_info.type == Item::Type::weapon ? weapon_weight : equipment_weight;
            weight -= item_info.weight;
            ecs.emplace_event<UnequipEvent>(hotbar_slot.item_id, idx);
            return i;
        }
    }

    return -1;
}

int InventoryComponent::pickup(wheel::ID item_id, int num) {
    for (auto [i, slot] : std::views::enumerate(backpack)) {
        if (slot.item_id == item_id) {
            slot.num += num;
            return i;
        }
    }
    for (auto [i, slot] : std::views::enumerate(backpack)) {
        if (slot.item_id == "") {
            slot.item_id = item_id;
            slot.num = num;
            return i;
        }
    }
    return -1;
}

int InventoryComponent::combine(int idx) {
    // TODO: optimize hard code 10 hear
    auto& slot = idx < 10 ? hotbar[idx] : backpack[idx - 10];
    if (slot.item_id == "") {
        return -1;
    }

    int hotbar_item_idx = -1;
    for (auto [i, hotbar_slot] : std::views::enumerate(hotbar)) {
        if (hotbar_slot.item_id == slot.item_id) {
            hotbar_item_idx = i;
            break;
        }
    }
    for (auto [i, backpack_slot] : std::views::enumerate(backpack)) {
        std::cout << backpack_slot.item_id << " " << slot.item_id << std::endl;
        if (backpack_slot.item_id == slot.item_id) {
            if (backpack_slot.num + (hotbar_item_idx != -1) >= 3) {
                if (!ItemManager::instance().has(backpack_slot.item_id + 1)) {
                    return -1;
                }
                if (hotbar_item_idx != -1) {
                    // TODO: optimize: maybe hash conflict
                    hotbar[hotbar_item_idx].item_id = hotbar[hotbar_item_idx].item_id + 1;
                    backpack_slot.num -= 2;
                } else {
                    wheel::ID new_item_id = slot.item_id + 1;
                    backpack_slot.num -= 3;
                    if (backpack_slot.num == 0) {
                        backpack_slot.item_id = "";
                    }
                    i = pickup(new_item_id, 1);
                }
                return idx != hotbar_item_idx && hotbar_item_idx != -1 ? hotbar_item_idx : i + 10;
            }
        }
    }
    return -1;
}

}  // namespace core
