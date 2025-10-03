#pragma once

#include <ecs/entity.hpp>

namespace core {

struct RemoveEntityEvent {
    wheel::Entity entity;
};

}  // namespace core
