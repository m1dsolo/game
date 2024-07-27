#pragma once

#include <wheel/ecs.hpp>

#include <game/global.hpp>

namespace wheel {

// 1. master get exp
// 2. master get kill stats
struct MasterComponent {
    MasterComponent(Entity master) : entity(master) {
        while (ecs.has_components<MasterComponent>(entity)) {
            entity = ecs.get_component<MasterComponent>(entity).entity;
        }
    }
    Entity entity;
};

}  // namespace wheel

