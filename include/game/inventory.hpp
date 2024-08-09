#pragma once

#include <memory>

#include <game/slot.hpp>

#include <game/component/actions.hpp>
#include <game/component/input.hpp>

namespace wheel {

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

    void swap(int idx1, int idx2);

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
