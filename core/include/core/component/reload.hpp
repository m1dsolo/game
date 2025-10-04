#pragma once

#include <wheel/timer.hpp>

namespace core {

struct ReloadComponent {
    ReloadComponent(int ammo, time_t reload_time, const std::string& sound_name = "")
        : current_ammo(ammo), max_ammo(ammo), reload_time(reload_time), sound_name(sound_name) {}

    int current_ammo;
    int max_ammo;
    time_t reload_time = 1000000;
    std::string sound_name;
    bool is_reloading = false;
};

}  // namespace core
