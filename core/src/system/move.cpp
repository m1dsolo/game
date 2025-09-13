#include <core/system/move.hpp>
#include <core/global.hpp>
#include <core/manager/time.hpp>
#include <core/manager/collider.hpp>
#include <core/component/transform.hpp>
#include <core/component/speed.hpp>
#include <core/component/direction.hpp>
#include <core/component/collider.hpp>

namespace core {

void MoveSystem::update_impl() {
    auto& time_manager = TimeManager::instance();
    auto time = static_cast<float>(time_manager.dt()) / time_manager.timer().TIME_UNIT_PER_SECOND;
    for (auto [entity, transform, speed, direction]
        : ecs.get_entity_and_components<TransformComponent, SpeedComponent, DirectionComponent>()) {
        auto delta = direction.move * speed.speed * time;

        auto& collider_manager = ColliderManager::instance();
        if (!ecs.has_components<ColliderComponent>(entity)) {
            transform.local.position += delta;
        } else {
            transform.global.position[0] += delta[0];
            if (collider_manager.is_colliding(entity)) {
                transform.global.position[0] -= delta[0];
            } else {
                transform.local.position[0] += delta[0];
            }
            transform.global.position[1] += delta[1];
            if (collider_manager.is_colliding(entity)) {
                transform.global.position[1] -= delta[1];
            } else {
                transform.local.position[1] += delta[1];
            }
            // if still collide, you can move
            if (collider_manager.is_colliding(entity)) {
                transform.local.position += delta;
            }
        }
    }
}

}  // namespace core
