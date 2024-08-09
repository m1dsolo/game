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
#include <game/component/collider.hpp>

namespace wheel {

void MoveSystem::execute_impl() {
    calc_input_direction();
    calc_direction_by_track_nearest_player();
    calc_direction_by_a_star_track_nearest_player();
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

void MoveSystem::calc_direction_by_track_nearest_player() {
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

void MoveSystem::calc_direction_by_a_star_track_nearest_player() {
    static int counter = 0;
    if (counter++ % 10) {
        return;
    }
    auto& map = Map::instance();
    for (auto [_, position0, velocity0, direction0]
        : ecs.get_components<AStarTrackNearestPlayerTag, PositionComponent, VelocityComponent, DirectionComponent>()) {
        Vector2D<double> pos = {1000000, 1000000};
        auto& pos0 = position0.vec;
        double min_distance = pos0.distance(pos);
        for (auto [_, position1] : ecs.get_components<PlayerComponent, PositionComponent>()) {
            auto& pos1 = position1.vec;
            double distance = pos0.distance(pos1);
            if (distance < min_distance) {
                min_distance = distance;
                pos = pos1;
            }
        }

        auto s = map.pos2idx(position0.vec);
        auto t = map.pos2idx(pos);
        auto path = a_star(s, t);
        if (path.size() >= 2) {
            pos = map.idx2pos(path[1].first, path[1].second);
        }
        direction0.vec = (pos - pos0).normalize();
    }
}

void MoveSystem::move() {
    auto& map = Map::instance();
    for (auto [entity, position, size, velocity, direction] : ecs.get_entity_and_components<PositionComponent, SizeComponent, VelocityComponent, DirectionComponent>()) {
        auto delta = direction.vec.normalize() * velocity.speed * timer_resource.delta / 1000000;
        if (!ecs.has_components<ColliderComponent>(entity)) {
            position.vec += delta;
            continue;
        }

        auto collide_size = Vector2D<double>{size.w / 2., size.h / 2.};
        position.vec.x += delta.x;
        if (map.is_collision({position.vec, collide_size})) {
            position.vec.x -= delta.x;
        }
        position.vec.y += delta.y;
        if (map.is_collision({position.vec, collide_size})) {
            position.vec.y -= delta.y;
        }
        if (map.is_collision({position.vec, collide_size})) {
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
