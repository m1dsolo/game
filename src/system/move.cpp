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
    for (auto [_, enemy, position0, velocity0, direction0]
        : ecs.get_components<TrackNearestEnemyTag, EnemyComponent, PositionComponent, VelocityComponent, DirectionComponent>()) {
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
    for (auto [position, velocity, direction] : ecs.get_components<PositionComponent, VelocityComponent, DirectionComponent>()) {
        position.vec += direction.vec.normalize() * velocity.speed * timer_resource.delta / 1000000;
    }
}

// TODO: cache
void MoveSystem::clamp_player_position() {
    auto& map = Map::instance();
    int map_w = map.dst().w;
    int map_h = map.dst().h;
    for (auto [player, position, size] : ecs.get_components<PlayerComponent, PositionComponent, SizeComponent>()) {
        int x0 = (config_resource.w - map_w + size.w) / 2;
        int y0 = (config_resource.h - map_h + size.h) / 2;
        int x1 = x0 + map_w;
        int y1 = y0 + map_h;
        position.vec.clamp(x0, x1 - size.w, y0, y1 - size.h);
    }
}

}  // namespace wheel
