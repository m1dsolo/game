#include <core/system/move.hpp>
#include <core/global.hpp>
#include <core/manager/collider.hpp>
#include <core/component/transform.hpp>
#include <core/component/speed.hpp>
#include <core/component/direction.hpp>
#include <core/component/collider.hpp>
#include <core/resource/time.hpp>
#include <core/tag/input.hpp>

namespace core {

void MoveSystem::operator()(wheel::ECS& ecs) {
    // update player direction from input
    auto player_entity = ecs.get_entity<InputTag>();
    if (ecs.has_component<DirectionComponent>(player_entity)) {
        auto& direction = ecs.get_component<DirectionComponent>(player_entity);
        auto d = wheel::Vector2D<float>{
            static_cast<float>(is_move_right) - is_move_left,
            static_cast<float>(is_move_down) - is_move_up,
        }.normalize();
        direction.move = d;
        if (d != 0.f) {
            direction.look = d;
        }
    }

    // move
    auto dt = static_cast<float>(ecs.get_resource<TimeResource>().dt) / timer.TIME_UNIT_PER_SECOND;
    for (auto [entity, transform, speed, direction]
        : ecs.get_entity_and_components<TransformComponent, SpeedComponent, DirectionComponent>()) {
        auto delta = direction.move * speed.speed * dt;

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
