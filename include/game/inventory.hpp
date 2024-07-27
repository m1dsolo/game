#pragma once

#include <memory>

#include <game/item/item.hpp>

#include <game/component/actions.hpp>
#include <game/component/input.hpp>

namespace wheel {

class Slot {
public:
    Slot() : item_(null_item_) {}
    Slot(std::shared_ptr<Item> item, int count = 1) : item_(item), count_(count) {}

    bool empty() const { return !item_; }

    Item& item() { return *item_; }
    const Item& item() const { return *item_; }
    int& count() { return count_; }
    const int& count() const { return count_; }

    void set(std::shared_ptr<Item> item, int count = 1);

    void clear();

private:
    std::shared_ptr<Item> item_;
    int count_ = 0;

    static const inline std::shared_ptr<Item> null_item_;
};

class Inventory {
public:
    Inventory(Entity entity);

    bool pick(const std::string& name, int count = 1);

    void drop(int idx);

    Slot& slot(int idx);

    Slot& selected_slot();

    void select(int idx);

    bool have(int idx);

    int selected_idx() const { return selected_idx_; }

private:
    static const inline int SIZE = 10;

    Entity entity_;
    Slot slots_[SIZE + 1];  // 0 is not used
    int selected_idx_ = 0;
};

}  // namespace wheel
