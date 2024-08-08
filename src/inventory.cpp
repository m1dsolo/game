#include <game/inventory.hpp>

#include <game/utils.hpp>
#include <game/item_manager.hpp>

namespace wheel {

void Slot::set(std::shared_ptr<Item> item, int count) {
    item_ = item;
    count_ = count;
}

void Slot::reduce(int count) {
    count_ -= count;
    if (count_ <= 0) {
        clear();
    }
}

void Slot::clear() {
    if (!item_->empty()) {
        item_->unselect();
    }
    item_ = null_item_;
    count_ = 0;
}

Inventory::Inventory(Entity entity) : entity_(entity) {
    for (int i = 0; i <= SIZE; i++) {
        slots_[i] = std::make_shared<Slot>();
    }
}

bool Inventory::pick(const std::string& name, int count) {
    if (ItemManager::instance().get_data(name)->stackable && pick_stackable(name, count)) {
        return true;
    }
    return pick_unstackable(ItemManager::instance().create_item(name, entity_), count);
}

bool Inventory::pick(std::shared_ptr<Item> item, int count) {
    if (item->data().stackable && pick_stackable(item->data().name, count)) {
        return true;
    }
    return pick_unstackable(item, count);
}

bool Inventory::pick_stackable(const std::string& name, int count) {
    for (int i = 1; i <= SIZE; i++) {
        if (!slots_[i]->empty() && slots_[i]->item().data().name == name) {
            slots_[i]->count() += count;
            return true;
        }
    }

    return false;
}

bool Inventory::pick_unstackable(std::shared_ptr<Item> item, int count) {
    for (int i = 1; i <= SIZE; i++) {
        if (slots_[i]->empty()) {
            slots_[i]->set(item, count);
            return true;
        }
    }

    return false;
}

std::pair<std::shared_ptr<Item>, int> Inventory::drop(int idx, int count) {
    auto& slot = *slots_[idx];
    count = std::min(slot.count(), count);
    auto item_ptr = slot.item_ptr();
    slot.reduce(count);

    return {item_ptr, count};
}

Slot& Inventory::slot(int idx) {
    return *slots_[idx];
}

Slot& Inventory::selected_slot() {
    return *slots_[selected_idx_];
}

void Inventory::select(int idx) {
    if (selected_idx_ == idx) {
        return;
    }

    // unselect
    if (have(selected_idx_)) {
        slots_[selected_idx_]->item().unselect();
    }

    // select
    if (have(idx)) {
        slots_[idx]->item().select();
    }

    selected_idx_ = idx;
}

bool Inventory::have(int idx) {
    return !slots_[idx]->empty();
}

}  // namespace wheel
