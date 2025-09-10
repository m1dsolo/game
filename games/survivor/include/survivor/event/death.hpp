#pragma once

#include <ecs/entity.hpp>

namespace survivor {

struct DeathEvent {
    wheel::Entity source;
    wheel::Entity target;
};

}  // namespace survivor
