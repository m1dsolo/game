#pragma once

#include <ecs/entity.hpp>

namespace survivor {

struct HPChangeEvent {
    wheel::Entity source;
    wheel::Entity target;
    int value;
};

}  // namespace survivor
