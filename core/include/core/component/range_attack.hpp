#pragma once

#include <wheel/timer.hpp>
#include <wheel/id.hpp>

namespace core {

struct RangeAttackComponent {
    int damage = 10;
    int range = 500.f;
    wheel::time_t interval = 1000000;
    float projectile_speed = 300.f;
    wheel::ID projectile_sprite_id;
    wheel::ID range_attack_sound_id;

    wheel::time_t time = 0;
};

}  // namespace core
