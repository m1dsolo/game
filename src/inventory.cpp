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
    auto* data = ItemManager::instance().get_data(name);
    // stack
    if (data->stackable) {
        for (int i = 1; i <= SIZE; i++) {
            if (!slots_[i]->empty() && slots_[i]->item().data().name == name) {
                slots_[i]->count() += count;
                return true;
            }
        }
    }

    // find empty slot
    for (int i = 1; i <= SIZE; i++) {
        if (slots_[i]->empty()) {
            slots_[i]->set(ItemManager::instance().create_item(name, entity_, slots_[i].get()), count);
            return true;
        }
    }

    return false;
}

void Inventory::drop(int idx) {
    slots_[idx]->clear();
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
