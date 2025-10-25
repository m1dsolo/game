#pragma once

#include <wheel/id.hpp>

namespace core {

struct ItemComponent {
    wheel::ID id;
    int count = 1;
};

}  // namespace core
