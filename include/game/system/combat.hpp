#pragma once

#include <wheel/singleton.hpp>

namespace wheel {

class CombatSystem : public Singleton<CombatSystem> {
    friend class Singleton<CombatSystem>;

public:
    void execute();
     
private:
    CombatSystem() = default;
    ~CombatSystem() = default;
    CombatSystem(const CombatSystem&) = delete;

    void damage();
    void player_get_damage_add_invincible();
    void get_damage_add_sketch();
    void enemy_get_damage_show_text();
    void dead();
    void add_exp();
    void del_damage_components();
    void del_exp_components();

    void del_useless_health_bar();
    void update_health_bar_position();
};

}  // namespace wheel
