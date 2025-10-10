#include <core/system/range_attack.hpp>
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
#include <core/component/projectile.hpp>
#include <core/component/range_attack.hpp>
#include <core/component/reload.hpp>
#include <core/component/hp.hpp>
#include <core/component/fraction.hpp>
#include <core/component/master.hpp>
#include <core/component/master.hpp>
#include <core/tag/obstacle.hpp>
#include <core/tag/render.hpp>
#include <core/resource/trigger.hpp>
#include <core/event/hp_change.hpp>
#include <core/event/trigger.hpp>
#include <core/entity_event/remove_entity.hpp>

using namespace core;

namespace core {

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

    // shoot the closest target
    const auto& closest_target = ecs.get_resource<TriggerResource>().closest_target;
    for (auto [trigger, target_pair] : closest_target) {
        if (!ecs.has_component<RangeAttackComponent>(trigger)) {
            continue;
        }
        auto [target, distance] = target_pair;
        if (!ecs.has_entity(target)) {
            continue;
        }

        auto& range_attack = ecs.get_component<RangeAttackComponent>(trigger);
        if (range_attack.time < range_attack.interval) {
            continue;
        }
        range_attack.time -= range_attack.interval;

        if (ecs.has_component<ReloadComponent>(trigger)) {
            auto& reload = ecs.get_component<ReloadComponent>(trigger);
            if (reload.current_ammo == 0) {
                continue;
            }
            if (--reload.current_ammo == 0) {
                AudioManager::instance().play(reload.sound_name);
                TimeManager::instance().timer().add(reload.reload_time, [trigger]() {
                    if (ecs.has_component<ReloadComponent>(trigger)) {
                        auto& reload = ecs.get_component<ReloadComponent>(trigger);
                        reload.current_ammo = reload.max_ammo;
                    }
                    return 0;
                });
            }
        }

        auto master = ecs.get_component<MasterComponent>(trigger).entity;
        auto fraction = ecs.get_component<FractionComponent>(master).fraction;
        const auto& target_pos = ecs.get_component<TransformComponent>(target).global.position;
        const auto& trigger_pos = ecs.get_component<TransformComponent>(trigger).global.position;

        EntityManager::instance().add_entity(
            NameComponent{"bullet"},
            ProjectileComponent{range_attack.damage},
            TransformComponent{trigger_pos, {16.f, 16.f}},
            ColliderComponent{
                wheel::Circle<float>{0.f, 0.f, 8.f},
                ColliderLayer::Projectile,
                ColliderLayer::Enemy | ColliderLayer::Obstacle
            },
            DirectionComponent{(target_pos - trigger_pos).normalize()},
            SpriteComponent{range_attack.projectile_sprite_name},
            SpeedComponent{range_attack.projectile_speed},
            RenderComponent{2},
            // TrackComponent{closest.first},
            FractionComponent{fraction},
            MasterComponent{master},
            RenderTag{}
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
                ecs.add_entity_event(entity, RemoveEntityEvent{});
            }

            // hit obstacle
            if (ecs.has_component<ObstacleTag>(target)) {
                ecs.add_entity_event(entity, RemoveEntityEvent{});
            }
        }
    }
}

}  // namespace core
