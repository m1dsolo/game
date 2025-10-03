#pragma once

#include <ecs/entity.hpp>

namespace core {

struct DeathEvent {
    wheel::Entity source;
    wheel::Entity target;
};

}  // namespace core
