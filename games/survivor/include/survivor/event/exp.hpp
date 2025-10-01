#pragma once

#include <ecs/entity.hpp>

namespace survivor {

struct ExpEvent {
    wheel::Entity entity;
    int exp;
};

}  // namespace survivor
