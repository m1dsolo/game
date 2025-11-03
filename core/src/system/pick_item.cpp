#include <core/system/pick_item.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
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
#include <core/tag/game_layer.hpp>
#include <core/event/trigger.hpp>
#include <core/event/exp.hpp>
#include <core/entity_event/remove_entity.hpp>
#include <core/sdl_user_event/update_slot.hpp>

#include <ecs/ecs.hpp>

namespace core {

void pickup_item(wheel::Entity new_item_entity);

void PickItemSystem::operator()(wheel::ECS& ecs) {
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
            if (item.id == "coin") {
                if (ecs.has_component<LevelComponent>(entity)) {
                    ecs.emplace_event<ExpEvent>(entity, 1);
                }
            } else {
                pickup_item(target);
            }
            ecs.add_entity_event(target, RemoveEntityEvent{});
        }
    }
}

void pickup_item(wheel::Entity new_item_entity) {
    const auto& new_item = ecs.get_component<ItemComponent>(new_item_entity);
    auto idx = ecs.get_component<InventoryComponent>().pickup(new_item.id, new_item.num);

    if (idx != -1) {
        SDL_Event event;
        event.type = SDL_EVENT_UPDATE_SLOT;
        event.user.code = idx;
        SDL_PushEvent(&event);
    }
}

}  // namespace core
