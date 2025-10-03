#pragma once

#include <ecs/entity.hpp>

namespace core {

struct ExpEvent {
    wheel::Entity entity;
    int exp;
};

}  // namespace core
