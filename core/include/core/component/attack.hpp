#pragma once

#include <wheel/timer.hpp>

namespace core {

struct AttackComponent {
    int damage = 0;
    time_t interval = 1000000;

    time_t time = 0;
};

}  // namespace core
