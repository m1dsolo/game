#pragma once

#include <game/system/base.hpp>

namespace wheel {

class CombatSystem : public BaseSystem<CombatSystem> {
    friend class BaseSystem<CombatSystem>;

public:
    void execute_impl() override;
     
private:
    CombatSystem() : BaseSystem(false) {}
    ~CombatSystem() = default;
    CombatSystem(const CombatSystem&) = delete;

    void damage();
    void player_get_damage_add_invincible();
    void get_damage_add_sketch();
    void enemy_get_damage_show_text();
    void heal_buff();
    void heal_show_text();
    void send_enemy_dead_event();
    void dead();
    void add_exp();
    void del_damage_components();
    void del_exp_components();
    void del_heal_buff_components();
    void del_useless_health_bar();
};

}  // namespace wheel
