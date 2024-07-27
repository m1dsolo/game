#pragma once

#include <wheel/ecs.hpp>

namespace wheel {

struct DelEntityTag {};

struct DelComponentTag {
    Entity entity;
    ComponentID component_id;
};

}  // namespace wheel
