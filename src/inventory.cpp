#include <game/inventory.hpp>

#include <game/utils.hpp>
#include <game/item_manager.hpp>
#include <game/audio_manager.hpp>

namespace wheel {

void Slot::set(std::shared_ptr<Item> item, int count) {
    item_ = item;
    count_ = count;
}

void Slot::clear() {
    item_ = null_item_;
    count_ = 0;
}

Inventory::Inventory(Entity entity) : entity_(entity) {
}

bool Inventory::pick(const std::string& name, int count) {
    auto* data = ItemManager::instance().get_data(name);
    // stack
    if (data->stackable) {
        for (int i = 1; i <= SIZE; i++) {
            if (!slots_[i].empty() && slots_[i].item().data().name == name) {
                slots_[i].count() += count;
                return true;
            }
        }
    }

    // find empty slot
    for (int i = 1; i <= SIZE; i++) {
        if (slots_[i].empty()) {
            slots_[i].set(ItemManager::instance().create_item(name, entity_), count);
            return true;
        }
    }

    return false;
}

void Inventory::drop(int idx) {
    slots_[idx].clear();
}

Slot& Inventory::slot(int idx) {
    return slots_[idx];
}

Slot& Inventory::selected_slot() {
    return slots_[selected_idx_];
}

void Inventory::select(int idx) {
    if (selected_idx_ == idx) {
        return;
    }

    selected_idx_ = idx;
    // unselect
    auto item = slots_[selected_idx_].item();
    if (!item.empty()) {
        if (ecs.has_components<ActionsComponent>(entity_)) {
            auto& actions = ecs.get_component<ActionsComponent>(entity_);
            for (auto& [name, action] : item.data().action_map) {
                actions.action_map.erase(name);
            }
        }
        if (ecs.has_components<InputComponent>(entity_)) {
            auto& input = ecs.get_component<InputComponent>(entity_);
            for (auto& [name, action] : item.data().action_map) {
                input.key_bindings.erase(action->keycode());
            }
        }
    }

    item = slots_[idx].item();
    if (ecs.has_components<ActionsComponent>(entity_)) {
        auto& actions = ecs.get_component<ActionsComponent>(entity_);
        for (auto& [name, action] : item.data().action_map) {
            actions.action_map[name] = action;
        }
    }
    if (ecs.has_components<InputComponent>(entity_)) {
        auto& input = ecs.get_component<InputComponent>(entity_);
        for (auto& [name, action] : item.data().action_map) {
            input.key_bindings[action->keycode()] = action->name();
        }
    }

    AudioManager::instance().play(item.data().name + "_select");
}

bool Inventory::have(int idx) {
    return !slots_[idx].empty();
}

}  // namespace wheel
