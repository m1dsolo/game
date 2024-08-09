#pragma once

#include <string>
#include <functional>

#include <wheel/singleton.hpp>

#include <game/global.hpp>
#include <game/component/buff.hpp>
#include <game/component/hp.hpp>
#include <game/component/velocity.hpp>
#include <game/component/perk.hpp>
#include <game/component/collider.hpp>

namespace wheel {

using Buff = std::function<void(Entity)>;

// TODO: reflection
class BuffManager : public Singleton<BuffManager> {
    friend class Singleton<BuffManager>;

public:
    template <typename... Args>
    Buff create(const std::string& name, Args&&... args) {
        auto args_tuple = std::make_tuple(std::forward<Args>(args)...);
        if (name == "hp") {
            int hp = std::get<0>(args_tuple);
            return [hp](Entity entity) {
                ecs.add_components(entity, HealBuffComponent{hp});
            };
        } else if (name == "max_hp") {
            int max_hp = std::get<0>(args_tuple);
            return [max_hp](Entity entity) {
                auto& hp = ecs.get_component<HPComponent>(entity);
                hp.max_hp += max_hp;
            };
        } else if (name == "max_speed") {
            int max_speed = std::get<0>(args_tuple);
            int seconds = -1;
            if constexpr (sizeof...(args) == 2) {
                seconds = std::get<1>(args_tuple);
            }
            return [max_speed, seconds](Entity entity) {
                auto& velocity = ecs.get_component<VelocityComponent>(entity);
                velocity.max_speed += max_speed;
                if (seconds != -1) {
                    timer_resource.add(config_resource.fps * seconds, 1, [entity, max_speed]() {
                        auto& velocity = ecs.get_component<VelocityComponent>(entity);
                        velocity.max_speed -= max_speed;
                    });
                }
            };
        } else if (name == "atk_ratio") {
            int atk_ratio = std::get<0>(args_tuple);
            return [atk_ratio](Entity entity) {
                auto& perk = ecs.get_component<PerkComponent>(entity);
                perk.atk_ratio += atk_ratio;
            };
        } else if (name == "shoot_speed_ratio") {
            int shoot_speed_ratio = std::get<0>(args_tuple);
            return [shoot_speed_ratio](Entity entity) {
                auto& perk = ecs.get_component<PerkComponent>(entity);
                perk.shoot_speed_ratio += shoot_speed_ratio;
            };
        } else if (name == "reload_speed_ratio") {
            int reload_speed_ratio = std::get<0>(args_tuple);
            return [reload_speed_ratio](Entity entity) {
                auto& perk = ecs.get_component<PerkComponent>(entity);
                perk.reload_speed_ratio += reload_speed_ratio;
            };
        } else if (name == "accuracy_ratio") {
            int accuracy_ratio = std::get<0>(args_tuple);
            return [accuracy_ratio](Entity entity) {
                auto& perk = ecs.get_component<PerkComponent>(entity);
                perk.accuracy_ratio += accuracy_ratio;
            };
        } else if (name == "pick_range") {
            int pick_range = std::get<0>(args_tuple);
            return [pick_range](Entity entity) {
                auto& perk = ecs.get_component<PerkComponent>(entity);
                perk.pick_range += pick_range;
            };
        } else if (name == "passwall") {
            int seconds = std::get<0>(args_tuple);
            return [seconds](Entity entity) {
                if (!ecs.has_components<ColliderComponent>()) {
                    return;
                }
                ecs.del_components<ColliderComponent>(entity);
                timer_resource.add(config_resource.fps * seconds, 1, [entity]() {
                    ecs.add_components(entity, ColliderComponent{});
                });
            };
        }
        return nullptr;
    }

private:
    BuffManager() = default;
    ~BuffManager() = default;
    BuffManager(const BuffManager&) = delete;
};

}  // namespace wheel
