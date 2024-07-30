#include <game/system/combat.hpp>

#include <game/global.hpp>
#include <game/ui.hpp>
#include <game/entity_manager.hpp>

#include <game/layer/card.hpp>
#include <game/event.hpp>

#include <game/component/hp.hpp>
#include <game/component/damage.hpp>
#include <game/component/invincible.hpp>
#include <game/component/tag.hpp>
#include <game/component/reward.hpp>
#include <game/component/level.hpp>
#include <game/component/player.hpp>
#include <game/component/self.hpp>
#include <game/component/animation.hpp>
#include <game/component/position.hpp>
#include <game/component/enemy.hpp>
#include <game/component/velocity.hpp>
#include <game/component/health_bar.hpp>
#include <game/component/size.hpp>
#include <game/component/master.hpp>
#include <game/component/buff.hpp>
#include <game/component/texture.hpp>

namespace wheel {

void CombatSystem::execute_impl() {
    damage();
    player_get_damage_add_invincible();
    get_damage_add_sketch();
    enemy_get_damage_show_text();
    heal_buff();
    heal_show_text();
    dead();
    add_exp();
    del_damage_components();
    del_exp_components();
    del_heal_buff_components();

    del_useless_health_bar();
    update_health_bar_position();
}

void CombatSystem::damage() {
    for (auto [entity, damage, hp] : ecs.get_entity_and_components<DamageComponent, HPComponent>()) {
        if (ecs.has_components<InvincibleComponent>(entity)) {
            continue;
        }

        for (auto& [d, attacker_entity] : damage.damages) {
            hp.hp -= d;
            if (hp.hp <= 0) {
                ecs.add_components<DeadComponent>(entity, {attacker_entity});
            }
        }
    }
}

void CombatSystem::player_get_damage_add_invincible() {
    for (auto [entity, player, damage] : ecs.get_entity_and_components<PlayerComponent, DamageComponent>()) {
        if (ecs.has_components<InvincibleComponent>(entity)) {
            continue;
        }
        ecs.add_components<InvincibleComponent>(entity, {1});
        timer_resource.add(30, 1, [entity]() {
            if (ecs.has_components<InvincibleComponent>(entity)) {
                auto& invincible = ecs.get_component<InvincibleComponent>(entity);
                if (!--invincible.layer) {
                    ecs.del_components<InvincibleComponent>(entity);
                }
            }
        });
    }
}

void CombatSystem::get_damage_add_sketch() {
    for (auto [entity, damage, animation] : ecs.get_entity_and_components<DamageComponent, AnimationComponent>()) {
        animation.sketch = true;
        timer_resource.add(10, 1, [entity]() {
            if (ecs.has_components<AnimationComponent>(entity)) {
                auto& animation = ecs.get_component<AnimationComponent>(entity);
                animation.sketch = false;
            }
        });
    }
}

void CombatSystem::enemy_get_damage_show_text() {
    for (auto [_, damage, position] : ecs.get_components<EnemyComponent, DamageComponent, PositionComponent>()) {
        for (auto& [d, _] : damage.damages) {
            Entity entity = EntityManager::instance().create_text(std::to_string(d), sdl.PINK, position.vec);
            timer_resource.add(20, 1, [entity]() {
                ecs.add_components<DelEntityTag>(entity, {});
            });
        }
    }
}

void CombatSystem::heal_buff() {
    for (auto [heal_buff, hp] : ecs.get_components<HealBuffComponent, HPComponent>()) {
        int heal = std::min(hp.max_hp - hp.hp, heal_buff.heal);
        hp.hp += heal;
        heal_buff.heal = heal;
    }
}

void CombatSystem::heal_show_text() {
    for (auto [heal_buff, position] : ecs.get_components<HealBuffComponent, PositionComponent>()) {
        Entity entity = EntityManager::instance().create_text(std::to_string(heal_buff.heal), sdl.GREEN, position.vec);
        timer_resource.add(20, 1, [entity]() {
            ecs.add_components<DelEntityTag>(entity, {});
        });
    }
}

void CombatSystem::dead() {
    for (auto [entity, dead] : ecs.get_entity_and_components<DeadComponent>()) {
        // del entity
        ecs.add_components<DelEntityTag>(entity, {});
        Entity killer = dead.killer_entity;
        Entity master = dead.killer_entity;
        if (ecs.has_components<MasterComponent>(master)) {
            master = ecs.get_component<MasterComponent>(master).entity;
        }

        // reward to master
        if (ecs.has_components<RewardComponent>(entity)) {
            auto& reward = ecs.get_component<RewardComponent>(entity);
            auto& position = ecs.get_component<PositionComponent>(entity);

            // add exp
            if (ecs.has_components<ExpComponent>(master)) {
                auto& exp = ecs.get_component<ExpComponent>(master);
                exp.exp += 1;
            } else {
                ecs.add_components<ExpComponent>(master, {1});
            }

            // drop item
            for (auto& reward_item : reward.reward_items) {
                if (random.uniform(0., 1.) < reward_item.chance) {
                    int count = random.uniform(reward_item.count.first, reward_item.count.second);
                    // position random offset
                    auto pos = Vector2D<double>{position.vec.x + random.uniform(-5., 5.), position.vec.y + random.uniform(-5., 5.)};
                    EntityManager::instance().create_item(reward_item.name, pos, count);
                }
            }
        }

        // kill stat to master player
        if (ecs.has_components<SelfComponent>(master)) {
            player_resource.killed++;
        }
    }
}

void CombatSystem::add_exp() {
    for (auto [entity, exp, level] : ecs.get_entity_and_components<ExpComponent, LevelComponent>()) {
        level.exp += exp.exp;
        if (level.exp >= config_resource.levelup_exps[level.level]) {
            level.exp -= config_resource.levelup_exps[level.level];
            level.level += 1;
            ecs.emplace_event<LevelUpEvent>(entity);
        }
    }
}

void CombatSystem::del_damage_components() {
    ecs.del_components<DamageComponent>();
}

void CombatSystem::del_exp_components() {
    ecs.del_components<ExpComponent>();
}

void CombatSystem::del_heal_buff_components() {
    ecs.del_components<HealBuffComponent>();
}

void CombatSystem::del_useless_health_bar() {
    for (auto [entity, health_bar] : ecs.get_entity_and_components<HealthBarComponent>()) {
        if (ecs.has_components<DelEntityTag>(health_bar.master)) {
            ecs.add_components<DelEntityTag>(entity, {});
        }
    }
}

void CombatSystem::update_health_bar_position() {
    for (auto [health_bar, bar_position, bar_size] : ecs.get_components<HealthBarComponent, PositionComponent, SizeComponent>()) {
        Entity entity = health_bar.master;
        auto& position = ecs.get_component<PositionComponent>(entity);
        auto& size = ecs.get_component<SizeComponent>(entity);

        bar_position.vec.x = position.vec.x - bar_size.w / 2.;
        bar_position.vec.y = position.vec.y - size.h / 2. - bar_size.h / 2.;
    }
}

}  // namespace wheel
