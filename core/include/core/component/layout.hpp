#pragma once

#include <ecs/entity.hpp>

#include <vector>

namespace core {

struct LayoutComponent {
    std::vector<std::vector<wheel::Entity>> widgets;
    std::pair<int, int> selected{0, 0};
    bool pressed = false;
};

}  // namespace core
