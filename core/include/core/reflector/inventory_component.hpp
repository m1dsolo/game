#pragma once

#include <rfl/internal/has_reflector.hpp>

#include <core/component/inventory.hpp>

#include <ecs/entity.hpp>

#include <vector>

namespace rfl {

template<>
struct Reflector<core::InventoryComponent> {
    using ReflType = std::vector<wheel::Entity>;

    static core::InventoryComponent to(const ReflType& value) {
        auto inventory = core::InventoryComponent(0);
        inventory.items = std::move(value);
        return inventory;
    }

    static ReflType from(const core::InventoryComponent& value) {
        return value.items;
    }
};

}  // namespace rfl
