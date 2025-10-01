#include <survivor/system/pick_item.hpp>
#include <core/global.hpp>
#include <core/manager/collider.hpp>
#include <core/component/transform.hpp>
#include <core/component/direction.hpp>
#include <core/component/speed.hpp>
#include <core/component/track.hpp>
#include <survivor/component/pick_item.hpp>
#include <survivor/component/inventory.hpp>
#include <survivor/component/item.hpp>
#include <survivor/component/level.hpp>
#include <survivor/event/exp.hpp>

using namespace core;

namespace survivor {

void PickItemSystem::operator()() {
    // absorb nearby items
    for (const auto [entity, transform, pick_item]
            : ecs.get_entity_and_components<TransformComponent, PickItemComponent>()) {
        auto position = transform.global.position;
        auto item_entities = ColliderManager::instance().query<ItemComponent>(entity);
        for (const auto& item_entity : item_entities) {
            auto item_position = ecs.get_component<TransformComponent>(item_entity).global.position;
            if (position.euclidean_distance(item_position) <= pick_item.radius) {
                ecs.add_components(
                    item_entity,
                    DirectionComponent{},
                    SpeedComponent{200.f},
                    TrackComponent{entity}
                );
            }
        }
    }

    // pick up items
    for (const auto [entity, inventory, _]
            : ecs.get_entity_and_components<InventoryComponent, PickItemComponent>()) {
        auto item_entities = ColliderManager::instance().query<ItemComponent>(entity);
        for (const auto& item_entity : item_entities) {
            const auto& item = ecs.get_component<ItemComponent>(entity);
            if (item.name == "coin") {
                if (ecs.has_component<LevelComponent>(entity)) {
                    ecs.emplace_event<ExpEvent>(entity, 1);
                }
            } else {

            }
        }
    }
}

}  // namespace survivor
