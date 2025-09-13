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
#include <core/event/del_entity.hpp>
#include <survivor/component/projectile.hpp>
#include <survivor/component/range_attack.hpp>
#include <survivor/component/master.hpp>
#include <survivor/component/hp.hpp>
#include <survivor/component/fraction.hpp>
#include <survivor/event/hp_change.hpp>

using namespace core;

namespace survivor {

void RangeAttackSystem::update_impl() {
    shoot_();
    collide_();
}

void RangeAttackSystem::shoot_() {
    for (auto [entity, range_attack, transform, master]
            : ecs.get_entity_and_components<RangeAttackComponent, TransformComponent, MasterComponent>()) {
        range_attack.time += TimeManager::instance().dt();
        if (range_attack.time < range_attack.interval) {
            continue;
        }
        range_attack.time -= range_attack.interval;

        auto fraction = ecs.get_component<FractionComponent>(master.entity).fraction;
        auto position = transform.global.position;

        auto iter = ColliderManager::instance().query(entity) |
            std::views::filter([&](auto other) {
                if (!ecs.has_components<FractionComponent, HPComponent, TransformComponent>(other)) {
                    return false;
                }
                if (ecs.get_component<FractionComponent>(other).fraction == fraction) {
                    return false;
                }
                return true;
            }) |
            std::views::transform([&](auto other) {
                const auto& other_position = ecs.get_component<TransformComponent>(other).global.position;
                return std::make_pair(other, position.euclidean_distance(other_position));
            });

        if (iter.begin() == iter.end()) {
            continue;
        }

        auto closest = *std::ranges::min_element(iter,
           [](const auto& a, const auto& b) {
               return a.second < b.second;
           });

        EntityManager::instance().add_entity(
            NameComponent{"bullet"},
            ProjectileComponent{range_attack.damage},
            TransformComponent{position, {16.f, 16.f}},
            TriggerComponent{wheel::Circle<float>{0.f, 0.f, 8.f}},
            DirectionComponent{(ecs.get_component<TransformComponent>(closest.first).global.position - position).normalize()},
            SpriteComponent{range_attack.projectile_sprite_name},
            SpeedComponent{range_attack.projectile_speed},
            RenderComponent{2},
            TrackComponent{closest.first},
            FractionComponent{fraction},
            MasterComponent{entity}
        );

        AudioManager::instance().play(range_attack.range_attack_sound_name);
    }
}

void RangeAttackSystem::collide_() {
    for (auto [entity, projectile, fraction]
            : ecs.get_entity_and_components<ProjectileComponent, FractionComponent>()) {
        for (auto target : ColliderManager::instance().query(entity)) {
            if (ecs.has_components<HPComponent, FractionComponent>(target) &&
                ecs.get_component<FractionComponent>(target).fraction != fraction.fraction) {

                auto source = entity;
                if (!ecs.has_component<MasterComponent>(entity)) {
                    source = ecs.get_component<MasterComponent>(entity).entity;
                }

                ecs.emplace_event<HPChangeEvent>(source, target, -projectile.damage);
                ecs.emplace_event<DelEntityEvent>(entity);
                break;
            }
        }
    }
}

}  // namespace survivor
