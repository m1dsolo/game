#pragma once

#include <wheel/timer.hpp>

namespace core {
    struct Sprite;
}

namespace survivor {

struct RangeAttackComponent {
    int damage = 10;
    int range = 300;
    time_t interval = 1000000;
    float projectile_speed = 300.f;
    std::string projectile_sprite_name;

    time_t time = 0;
};

}  // namespace survivor
