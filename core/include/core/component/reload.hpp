#pragma once

#include <wheel/timer.hpp>
#include <wheel/id.hpp>

namespace core {

struct ReloadComponent {
    ReloadComponent(int ammo, time_t reload_time, wheel::ID sound_id = "")
        : current_ammo(ammo), max_ammo(ammo), reload_time(reload_time), sound_id(sound_id) {}

    int current_ammo;
    int max_ammo;
    time_t reload_time = 1000000;
    wheel::ID sound_id;
};

}  // namespace core
