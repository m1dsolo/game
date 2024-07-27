#include <game/system/tower.hpp>

#include <game/global.hpp>
#include <game/entity_manager.hpp>

#include <game/component/tower.hpp>
#include <game/component/position.hpp>
#include <game/component/direction.hpp>
#include <game/component/actions.hpp>
#include <game/component/enemy.hpp>

namespace wheel {

void TowerSystem::execute() {
    if (game_resource.paused) {
        return;
    }

    attack();
}

void TowerSystem::attack() {
    for (auto [tower_entity, tower, actions, tower_position, tower_direction]
        : ecs.get_entity_and_components<TowerComponent, ActionsComponent, PositionComponent, DirectionComponent>()) {

        // get nearest enemy position
        Vector2D<double> pos = {1000000, 1000000};
        double min_distance = tower_position.vec.distance(pos);
        for (auto [enemy, enemy_position] : ecs.get_components<EnemyComponent, PositionComponent>()) {
            double distance = tower_position.vec.distance(enemy_position.vec);
            if (distance < min_distance) {
                min_distance = distance;
                pos = enemy_position.vec;
            }
        }

        if (min_distance <= tower.range) {
            // aim
            tower_direction.vec = (pos - tower_position.vec).normalize();

            if (actions.action_map.count("shoot")) {
                actions.action_map["shoot"]->start();
                actions.action_map["shoot"]->finish();
            }
        } else {
            if (actions.action_map.count("reload")) {
                actions.action_map["reload"]->start();
                actions.action_map["reload"]->finish();
            }
        }
    }
}

}  // namespace wheel
