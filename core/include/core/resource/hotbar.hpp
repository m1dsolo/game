#pragma once

#include <ecs/entity.hpp>

#include <array>
#include <vector>

namespace core {

struct HotbarResource {
    std::array<std::vector<wheel::Entity>, 10> entities;
};

}  // namespace core
