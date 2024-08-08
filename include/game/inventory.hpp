#pragma once

#include <memory>

#include <game/item/item.hpp>

#include <game/component/actions.hpp>
#include <game/component/input.hpp>

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

    static const std::shared_ptr<Item> null_item_;
};

inline const std::shared_ptr<Item> Slot::null_item_ = std::make_shared<Item>();

class Inventory {
public:
    Inventory(Entity entity);

    bool pick(const std::string& name, int count = 1);
    bool pick(std::shared_ptr<Item> item, int count = 1);

    std::pair<std::shared_ptr<Item>, int> drop(int idx, int count = 1);

    Slot& slot(int idx);

    Slot& selected_slot();

    void select(int idx);
    
    void unselect(int idx);

    bool have(int idx);

    int selected_idx() const { return selected_idx_; }

private:
    static const inline int SIZE = 50;

    bool pick_stackable(const std::string& name, int count);
    bool pick_unstackable(std::shared_ptr<Item> item, int count);

    Entity entity_;
    std::shared_ptr<Slot> slots_[SIZE + 1];  // 0 is not used
    int selected_idx_ = 0;
};

}  // namespace wheel
