#pragma once

#include <ecs/entity.hpp>

namespace core {

struct TrackComponent {
    wheel::Entity target;
    float range = 0.f;
};

}  // namespace core
