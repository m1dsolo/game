#pragma once

#include <ecs/entity.hpp>

#include <vector>

namespace core {

struct ChildrenComponent {
    std::vector<wheel::Entity> children;
};

}  // namespace core
