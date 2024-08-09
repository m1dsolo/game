#pragma once

#include <memory>

#include <game/item/item.hpp>

namespace wheel {

class Slot {
    friend class Inventory;

public:
    Slot() : item_(null_item_) {}

    bool empty() const { return item_->empty(); }

    Item& item() { return *item_; }
    const Item& item() const { return *item_; }
    std::shared_ptr<Item> item_ptr() { return item_; }
    int& count() { return count_; }
    const int& count() const { return count_; }

    void set(std::shared_ptr<Item> item, int count = 1);

    void reduce(int count = 1);

    void clear();

private:
    std::shared_ptr<Item> item_;
    int count_ = 0;

    static inline const std::shared_ptr<Item> null_item_ = std::make_shared<Item>();
};

}  // namespace wheel
