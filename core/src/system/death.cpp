#include <core/system/death.hpp>
#include <core/global.hpp>
#include <core/manager/collider.hpp>
#include <core/component/animation_fsm.hpp>
#include <core/component/children.hpp>
#include <core/component/attack.hpp>
#include <core/component/track.hpp>
#include <core/component/speed.hpp>
#include <core/tag/rigidbody.hpp>
#include <core/entity_event/death.hpp>
#include <core/entity_event/remove_entity.hpp>

namespace core {

void DeathSystem::operator()() {
    for (auto [entity, event] : ecs.get_entity_and_components<DeathEvent>()) {
        if (ecs.has_component<AnimationFSMComponent>(entity)) {
            auto& animation_fsm = ecs.get_component<AnimationFSMComponent>(entity);
            animation_fsm.next_condition2values["is_dead"] = true;

            // TODO: optimize
            ecs.remove_components<TrackComponent, SpeedComponent, RigidbodyTag>(entity);
            ColliderManager::instance().remove(entity);
            if (ecs.has_component<ChildrenComponent>(entity)) {
                for (auto child : ecs.get_component<ChildrenComponent>(entity).entities) {
                    if (ecs.has_component<AttackComponent>(child)) {
                        ecs.add_entity_event(child, RemoveEntityEvent{});
                    }
                }
            }
        } else {
            ecs.add_entity_event(entity, RemoveEntityEvent{});
        }
    }

    // TODO: optimize(use tag?)
    for (auto [entity, animation_fsm] : ecs.get_entity_and_components<AnimationFSMComponent>()) {
        if (animation_fsm.current_state == "end") {
            ecs.add_entity_event(entity, RemoveEntityEvent{});
        }
    }
}

}  // namespace core
