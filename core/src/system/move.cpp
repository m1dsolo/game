#include <core/system/move.hpp>
#include <core/global.hpp>
#include <core/manager/time.hpp>
#include <core/manager/collider.hpp>
#include <core/component/transform.hpp>
#include <core/component/speed.hpp>
#include <core/component/direction.hpp>
#include <core/component/collider.hpp>
#include <core/tag/input.hpp>

namespace core {

void MoveSystem::update_impl() {
    process_event_();
    update_move_direction_();
    move_();
}

void MoveSystem::process_event_() {
    for (const auto& event : ecs.get_events<SDL_Event>()) {
        switch (event.type) {
            case SDL_EVENT_KEY_DOWN: {
                switch (event.key.key) {
                    case SDLK_W: is_move_up_ = true; break;
                    case SDLK_S: is_move_down_ = true; break;
                    case SDLK_A: is_move_left_ = true; break;
                    case SDLK_D: is_move_right_ = true; break;
                }
                break;
            }
            case SDL_EVENT_KEY_UP: {
                switch (event.key.key) {
                    case SDLK_W: is_move_up_ = false; break;
                    case SDLK_S: is_move_down_ = false; break;
                    case SDLK_A: is_move_left_ = false; break;
                    case SDLK_D: is_move_right_ = false; break;
                }
                break;
            }
        }
    }
}

void MoveSystem::update_move_direction_() {
    if (ecs.has_component<InputTag>()) {
        auto entity = ecs.get_entity<InputTag>();
        auto& direction = ecs.get_component<DirectionComponent>(entity);
        auto d = wheel::Vector2D<float>{
            static_cast<float>(is_move_right_) - is_move_left_,
            static_cast<float>(is_move_down_) - is_move_up_,
        }.normalize();
        direction.move = d;
        if (d != 0.f) {
            direction.look = d;
        }
    }
}

void MoveSystem::move_() {
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
