#pragma once

#include <ecs/entity.hpp>

namespace core {

struct HPChangeEvent {
    wheel::Entity source;
    wheel::Entity target;
    int value;
};

}  // namespace core
