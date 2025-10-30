#pragma once

#include <wheel/timer.hpp>
#include <wheel/id.hpp>

namespace core {

struct ReloadComponent {
    int current_ammo;
    int max_ammo;
    time_t reload_time = 1000000;
    wheel::ID sound_id;
};

}  // namespace core
