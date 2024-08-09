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
            auto [hp] = args_tuple;
            return [hp](Entity entity) {
                ecs.add_components(entity, HealBuffComponent{hp});
            };
        } else if (name == "max_hp") {
            auto [max_hp] = args_tuple;
            return [max_hp](Entity entity) {
                auto& hp = ecs.get_component<HPComponent>(entity);
                hp.max_hp += max_hp;
            };
        } else if (name == "max_speed") {
            auto [max_speed] = args_tuple;
            return [max_speed](Entity entity) {
                auto& velocity = ecs.get_component<VelocityComponent>(entity);
                velocity.max_speed += max_speed;
            };
        } else if (name == "atk_ratio") {
            auto [atk_ratio] = args_tuple;
            return [atk_ratio](Entity entity) {
                auto& perk = ecs.get_component<PerkComponent>(entity);
                perk.atk_ratio += atk_ratio;
            };
        } else if (name == "shoot_speed_ratio") {
            auto [shoot_speed_ratio] = args_tuple;
            return [shoot_speed_ratio](Entity entity) {
                auto& perk = ecs.get_component<PerkComponent>(entity);
                perk.shoot_speed_ratio += shoot_speed_ratio;
            };
        } else if (name == "reload_speed_ratio") {
            auto [reload_speed_ratio] = args_tuple;
            return [reload_speed_ratio](Entity entity) {
                auto& perk = ecs.get_component<PerkComponent>(entity);
                perk.reload_speed_ratio += reload_speed_ratio;
            };
        } else if (name == "accuracy_ratio") {
            auto [accuracy_ratio] = args_tuple;
            return [accuracy_ratio](Entity entity) {
                auto& perk = ecs.get_component<PerkComponent>(entity);
                perk.accuracy_ratio += accuracy_ratio;
            };
        } else if (name == "pick_range") {
            auto [pick_range] = args_tuple;
            return [pick_range](Entity entity) {
                auto& perk = ecs.get_component<PerkComponent>(entity);
                perk.pick_range += pick_range;
            };
        } else if (name == "passwall") {
            auto [seconds] = args_tuple;
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
