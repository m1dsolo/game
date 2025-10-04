#include <core/system/pick_item.hpp>
#include <core/global.hpp>
#include <core/manager/collider.hpp>
#include <core/component/transform.hpp>
#include <core/component/direction.hpp>
#include <core/component/speed.hpp>
#include <core/component/track.hpp>
#include <core/component/inventory.hpp>
#include <core/component/item.hpp>
#include <core/component/level.hpp>
#include <core/component/master.hpp>
#include <core/tag/absorb_item.hpp>
#include <core/tag/pickup_item.hpp>
#include <core/event/trigger.hpp>
#include <core/event/exp.hpp>
#include <core/entity_event/remove_entity.hpp>

using namespace core;

namespace core {

void PickItemSystem::operator()() {
    for (const auto [entity, target] : ecs.get_events<TriggerEnterEvent>()) {
        // absorb nearby items
        if (ecs.has_component<AbsorbItemTag>(entity) && ecs.has_component<ItemComponent>(target)) {
            ecs.add_components(
                target,
                DirectionComponent{},
                SpeedComponent{500.f},
                TrackComponent{entity}
            );
        }

        // pick up items
        if (ecs.has_component<PickupItemTag>(entity) && ecs.has_component<ItemComponent>(target)) {
            const auto& item = ecs.get_component<ItemComponent>(target);
            if (item.name == "coin") {
                if (ecs.has_component<LevelComponent>(entity)) {
                    ecs.emplace_event<ExpEvent>(entity, 1);
                }
            } else {
            }
            ecs.add_entity_event(target, RemoveEntityEvent{});
        }
    }
}

}  // namespace core
