#pragma once

#include <ecs/entity.hpp>

#include <vector>

namespace core {

struct ChildrenComponent {
    std::vector<wheel::Entity> entities;
};

}  // namespace core
