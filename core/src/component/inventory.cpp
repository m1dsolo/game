#include <core/component/inventory.hpp>
#include <core/global.hpp>
#include <core/manager/item.hpp>
#include <core/event/equip.hpp>

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

}  // namespace core
