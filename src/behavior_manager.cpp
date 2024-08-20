#include <game/behavior_manager.hpp>

#include <game/global.hpp>
#include <game/map.hpp>
#include <game/item/gun.hpp>

#include <game/component/inventory.hpp>
#include <game/component/tower.hpp>
#include <game/component/position.hpp>
#include <game/component/enemy.hpp>
#include <game/component/aim_direction.hpp>
#include <game/component/actions.hpp>

namespace wheel {

BehaviorManager::BehaviorManager() {
    behavior_map_["have_ammo"] = [&](Entity entity, BehaviorTree::Context& context) {
        auto& inventory = ecs.get_component<InventoryComponent>(entity).inventory;
        auto& gun = static_cast<Gun&>(inventory.selected_slot().item());
        if (gun.ammo() > 0) {
            return BehaviorTree::Node::State::SUCCESS;
        }
        return BehaviorTree::Node::State::FAILURE;
    };

    behavior_map_["enemy_in_range"] = [&](Entity entity, BehaviorTree::Context& context) {
        auto [tower, tower_position] = ecs.get_components<TowerComponent, PositionComponent>(entity);
        Vector2D<float> pos = {1000000, 1000000};
        float min_distance = tower_position.vec.distance(pos);
        for (auto [enemy, enemy_position] : ecs.get_components<EnemyComponent, PositionComponent>()) {
            if (!Map::is_in_bound(enemy_position.vec)) {
                continue;
            }
            float distance = tower_position.vec.distance(enemy_position.vec);
            if (distance < min_distance) {
                min_distance = distance;
                pos = enemy_position.vec;
            }
        }
        if (min_distance <= tower.range) {
            context["nearest_enemy_pos"] = std::move(pos);
            return BehaviorTree::Node::State::SUCCESS;
        } else {
            return BehaviorTree::Node::State::FAILURE;
        }
    };

    behavior_map_["aim_nearest_enemy"] = [&](Entity entity, BehaviorTree::Context& context) {
        const auto& target_pos = std::any_cast<Vector2D<float>>(context["nearest_enemy_pos"]);
        const auto& self_pos = ecs.get_component<PositionComponent>(entity).vec;
        auto& aim_direction = ecs.get_component<AimDirectionComponent>(entity).vec;
        aim_direction = (target_pos - self_pos).normalize();
        return BehaviorTree::Node::State::SUCCESS;
    };

    behavior_map_["shoot"] = [&](Entity entity, BehaviorTree::Context& context) {
        const auto& action_map = ecs.get_component<ActionsComponent>(entity).action_map;

        if (action_map.count("shoot")) {
            action_map.at("shoot")->start();
            action_map.at("shoot")->finish();
            return BehaviorTree::Node::State::SUCCESS;
        } else {
            return BehaviorTree::Node::State::FAILURE;
        }
    };

    behavior_map_["reload"] = [&](Entity entity, BehaviorTree::Context& context) {
        const auto& action_map = ecs.get_component<ActionsComponent>(entity).action_map;
        if (action_map.count("reload")) {
            action_map.at("reload")->start();
            action_map.at("reload")->finish();
            return BehaviorTree::Node::State::SUCCESS;
        } else {
            return BehaviorTree::Node::State::FAILURE;
        }
    };
}

BehaviorTree::Behavior BehaviorManager::get(const std::string& name) {
    return behavior_map_.at(name);
}

}  // namespace wheel
