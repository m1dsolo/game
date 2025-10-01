#include <survivor/system/pick_item.hpp>
#include <survivor/component/inventory.hpp>
#include <survivor/component/item.hpp>
#include <survivor/component/level.hpp>
#include <survivor/component/master.hpp>
#include <survivor/tag/absorb_item.hpp>
#include <survivor/tag/pickup_item.hpp>
#include <survivor/event/exp.hpp>

#include <core/global.hpp>
#include <core/manager/collider.hpp>
#include <core/component/transform.hpp>
#include <core/component/direction.hpp>
#include <core/component/speed.hpp>
#include <core/component/track.hpp>
#include <core/event/del_entity.hpp>
#include <core/event/trigger.hpp>

using namespace core;

namespace survivor {

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
            ecs.emplace_event<DelEntityEvent>(target);
        }
    }
}

}  // namespace survivor
