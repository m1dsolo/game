#include <game/system/move.hpp>

#include <game/global.hpp>
#include <game/map.hpp>
#include <game/collision_manager.hpp>
#include <game/scene_manager.hpp>

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
#include <game/component/health_bar.hpp>
#include <game/component/master.hpp>

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

        auto d = Vector2D<float>{(float)move.right - move.left, (float)move.down - move.up};
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
        Vector2D<float> pos = {1000000, 1000000};
        auto& pos0 = position0.vec;
        float min_distance = pos0.distance(pos);
        // TODO: performance
        for (auto [_, position1] : ecs.get_components<PlayerComponent, PositionComponent>()) {
            auto& pos1 = position1.vec;
            float distance = pos0.distance(pos1);
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
        Vector2D<float> pos = {1000000, 1000000};
        auto& pos0 = position0.vec;
        float min_distance = pos0.distance(pos);
        for (auto [_, position1] : ecs.get_components<PlayerComponent, PositionComponent>()) {
            auto& pos1 = position1.vec;
            float distance = pos0.distance(pos1);
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
    auto& collision_manager = CollisionManager::instance();
    auto& scene_manager = SceneManager::instance();
    for (auto [entity, position, velocity, direction] : ecs.get_entity_and_components<PositionComponent, VelocityComponent, DirectionComponent>()) {
        auto delta = direction.vec.normalize() * velocity.speed * timer_resource.delta / 1000000;
        if (!ecs.has_components<ColliderComponent>(entity)) {
            position.vec += delta;
        } else {
            position.vec.x += delta.x;
            if (collision_manager.is_collision_with_static(entity)) {
                position.vec.x -= delta.x;
            }
            position.vec.y += delta.y;
            if (collision_manager.is_collision_with_static(entity)) {
                position.vec.y -= delta.y;
            }
            // if still collide, you can move
            if (collision_manager.is_collision_with_static(entity)) {
                position.vec += delta;
            }
        }

        scene_manager.update(entity);
    }
}

// TODO: cache
void MoveSystem::clamp_player_position() {
    auto& map = Map::instance();
    auto [x0, y0, x1, y1] = map.game_rect();
    for (auto [player, position, size] : ecs.get_components<PlayerComponent, PositionComponent, SizeComponent>()) {
        auto [w, h] = size.vec;
        position.vec.clamp(
            x0 + w / 2,
            x1 - w / 2,
            y0 + h / 2,
            y1 - h / 2
        );
    }
}

}  // namespace wheel
