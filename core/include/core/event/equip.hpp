#pragma once

#include <wheel/id.hpp>

namespace core {

struct EquipEvent {
    wheel::ID item_id;
    int slot_idx;
};

struct UnequipEvent {
    wheel::ID item_id;
    int slot_idx;
};

}  // namespace core
