#include <survivor/system/range_attack.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/collider.hpp>
#include <core/manager/time.hpp>
#include <core/manager/audio.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/direction.hpp>
#include <core/component/speed.hpp>
#include <core/component/sprite.hpp>
#include <core/component/animation.hpp>
#include <core/component/render.hpp>
#include <core/component/track.hpp>
#include <core/event/trigger.hpp>
#include <core/event/del_entity.hpp>

#include <survivor/component/projectile.hpp>
#include <survivor/component/range_attack.hpp>
#include <survivor/component/hp.hpp>
#include <survivor/component/fraction.hpp>
#include <survivor/component/master.hpp>
#include <survivor/tag/obstacle.hpp>
#include <survivor/event/hp_change.hpp>
#include <survivor/collider_layer.hpp>

using namespace core;

namespace survivor {

void shoot_();
void collide_();

void RangeAttackSystem::operator()() {
    shoot_();
    collide_();
}

void shoot_() {
    // prepare shoot
    auto dt = TimeManager::instance().dt();
    for (auto [range_attack] : ecs.get_components<RangeAttackComponent>()) {
        if (range_attack.time < range_attack.interval) {
            range_attack.time += dt;
        } else {
            range_attack.time = range_attack.interval;
        }
    }

    // find targets in range
    // TODO: no need to check fraction for now because collider group already checked
    std::unordered_map<wheel::Entity, std::vector<wheel::Entity>> in_range_mp;
    for (const auto [trigger, target] : ecs.get_events<TriggerStayEvent>()) {
        if (ecs.has_component<RangeAttackComponent>(trigger) && ecs.has_component<HPComponent>(target)) {
            in_range_mp[trigger].emplace_back(target);
        }
    }

    // shoot the closest target
    for (const auto& [trigger, targets] : in_range_mp) {
        auto& range_attack = ecs.get_component<RangeAttackComponent>(trigger);
        if (range_attack.time < range_attack.interval) {
            continue;
        }
        range_attack.time -= range_attack.interval;

        const auto& trigger_pos = ecs.get_component<TransformComponent>(trigger).global.position;
        auto closest = *std::min_element(targets.begin(), targets.end(), [&](auto target0, auto target1) {
            const auto& target0_pos = ecs.get_component<TransformComponent>(target0).global.position;
            const auto& target1_pos = ecs.get_component<TransformComponent>(target1).global.position;
            return trigger_pos.euclidean_distance(target0_pos) < trigger_pos.euclidean_distance(target1_pos);
        });

        auto parent = ecs.get_component<ParentComponent>(trigger).entity;
        auto fraction = ecs.get_component<FractionComponent>(parent).fraction;
        EntityManager::instance().add_entity(
            NameComponent{"bullet"},
            ProjectileComponent{range_attack.damage},
            TransformComponent{trigger_pos, {16.f, 16.f}},
            ColliderComponent{
                wheel::Circle<float>{0.f, 0.f, 8.f},
                ColliderLayer::Projectile,
                ColliderLayer::Enemy | ColliderLayer::Obstacle
            },
            DirectionComponent{(ecs.get_component<TransformComponent>(closest).global.position - trigger_pos).normalize()},
            SpriteComponent{range_attack.projectile_sprite_name},
            SpeedComponent{range_attack.projectile_speed},
            RenderComponent{2},
            // TrackComponent{closest.first},
            FractionComponent{fraction},
            MasterComponent{parent}
        );

        AudioManager::instance().play(range_attack.range_attack_sound_name);
    }
}

void collide_() {
    for (const auto [entity, target] : ecs.get_events<TriggerStayEvent>()) {
        if (ecs.has_components<ProjectileComponent>(entity)) {
            auto& projectile = ecs.get_component<ProjectileComponent>(entity);

            // hit the target
            if (ecs.has_component<HPComponent>(target)) {
                if (projectile.last == target) {
                    continue;
                }
                projectile.last = target;

                auto master = ecs.get_component<MasterComponent>(entity).entity;
                ecs.emplace_event<HPChangeEvent>(master, target, -projectile.damage);
                ecs.emplace_event<DelEntityEvent>(entity);
            }

            // hit obstacle
            if (ecs.has_component<ObstacleTag>(target)) {
                ecs.emplace_event<DelEntityEvent>(entity);
            }
        }
    }
}

}  // namespace survivor
