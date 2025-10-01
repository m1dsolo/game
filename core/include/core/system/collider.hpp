#pragma once

#include <ecs/entity.hpp>

#include <unordered_set>
#include <unordered_map>

namespace core {

struct ColliderSystem {
    void operator()();

private:
    void helper_(wheel::Entity entity0, wheel::Entity entity1);

    std::unordered_map<wheel::Entity, std::unordered_set<wheel::Entity>> last_collided_entities_;
    std::unordered_map<wheel::Entity, std::unordered_set<wheel::Entity>> current_collided_entities_;
};

}  // namespace core
