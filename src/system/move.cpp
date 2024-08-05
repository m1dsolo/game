#include <game/system/move.hpp>

#include <game/global.hpp>
#include <game/map.hpp>

#include <game/component/move.hpp>
#include <game/component/position.hpp>
#include <game/component/velocity.hpp>
#include <game/component/direction.hpp>
#include <game/component/size.hpp>
#include <game/component/enemy.hpp>
#include <game/component/ai.hpp>
#include <game/component/player.hpp>
#include <game/component/continuous_action.hpp>

namespace wheel {

void MoveSystem::execute_impl() {
    calc_input_direction();
    calc_direction_by_track_nearest_enemy();
    move();
    clamp_player_position();
}

void MoveSystem::calc_input_direction() {
    // get input direction and speed
    for (auto [entity, move, velocity, direction] : ecs.get_entity_and_components<MoveComponent, VelocityComponent, DirectionComponent>()) {
        if (ecs.has_components<ContinuousActionComponent>(entity)) {
            continue;
        }

        auto d = Vector2D<double>{(double)move.right - move.left, (double)move.down - move.up};
        if (d.is_zero()) {
            velocity.speed = 0;
        } else {
            direction.vec = std::move(d);
            velocity.speed = velocity.max_speed;
        }
    }
}

void MoveSystem::calc_direction_by_track_nearest_enemy() {
    for (auto [_, position0, velocity0, direction0]
        : ecs.get_components<TrackNearestPlayerTag, PositionComponent, VelocityComponent, DirectionComponent>()) {
        Vector2D<double> pos = {1000000, 1000000};
        auto& pos0 = position0.vec;
        double min_distance = pos0.distance(pos);
        // TODO: performance
        for (auto [_, position1] : ecs.get_components<PlayerComponent, PositionComponent>()) {
            auto& pos1 = position1.vec;
            double distance = pos0.distance(pos1);
            if (distance < min_distance) {
                min_distance = distance;
                pos = pos1;
            }
        }

        direction0.vec = (pos - pos0).normalize();
    }
}

void MoveSystem::move() {
    auto& map = Map::instance();
    for (auto [position, size, velocity, direction] : ecs.get_components<PositionComponent, SizeComponent, VelocityComponent, DirectionComponent>()) {
        auto delta = direction.vec.normalize() * velocity.speed * timer_resource.delta / 1000000;
        position.vec.x += delta.x;
        if (map.is_collision({position.vec, {(double)size.w, (double)size.h}})) {
            position.vec.x -= delta.x;
        }
        position.vec.y += delta.y;
        if (map.is_collision({position.vec, {(double)size.w, (double)size.h}})) {
            position.vec.y -= delta.y;
        }
        if (map.is_collision({position.vec, {(double)size.w, (double)size.h}})) {
            position.vec += delta;
        }
    }
}

// TODO: cache
void MoveSystem::clamp_player_position() {
    auto& map = Map::instance();
    auto& [x0, y0, x1, y1] = map.game_rect();
    for (auto [player, position, size] : ecs.get_components<PlayerComponent, PositionComponent, SizeComponent>()) {
        position.vec.clamp(
            x0 + (float)size.w / 2,
            x1 - (float)size.w / 2,
            y0 + (float)size.h / 2,
            y1 - (float)size.h / 2
        );
    }
}

}  // namespace wheel
