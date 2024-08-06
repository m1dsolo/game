#pragma once

#include <vector>

#include <game/buff_manager.hpp>

namespace wheel {

struct EnemyResource {
    std::vector<Buff> buffs;

    int combat_seconds = 30;
    int enemy_num = 30;

    int max_hp_ratio = 100;
    int max_speed_ratio = 100;
    int extra_collide_damage = 0;

    int elite_chance = 5;
    int elite_max_hp_ratio = 1000;
    int elite_size_ratio = 200;
    int elite_extra_collide_damage = 0;
};

}  // namespace wheel
