#pragma once

#include <ecs/entity.hpp>

#include <unordered_map>

namespace core {

struct TriggerResource {
    std::unordered_map<wheel::Entity, std::pair<wheel::Entity, float>> closest_target;
};

}  // namespace core
