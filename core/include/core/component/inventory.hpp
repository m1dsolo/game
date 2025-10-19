#pragma once

#include <vector>

#include <ecs/entity.hpp>

namespace core {

struct InventoryComponent {
    InventoryComponent(int size) {
        items.resize(size, wheel::NullEntity);
    }
    std::vector<wheel::Entity> items;
};

}  // namespace core
