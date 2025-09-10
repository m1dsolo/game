#pragma once

#include <ecs/entity.hpp>

namespace core {

struct DelEntityEvent {
    wheel::Entity entity;
};

}  // namespace core
