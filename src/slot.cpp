#include <game/slot.hpp>

namespace wheel {

void Slot::set(std::shared_ptr<Item> item, int count) {
    item_ = item;
    count_ = count;
    item->set_slot(this);
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

}  // namespace wheel
