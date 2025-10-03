#include <core/system/death.hpp>
#include <core/global.hpp>
#include <core/manager/collider.hpp>
#include <core/component/animation_fsm.hpp>
#include <core/component/children.hpp>
#include <core/component/attack.hpp>
#include <core/component/track.hpp>
#include <core/component/speed.hpp>
#include <core/tag/rigidbody.hpp>
#include <core/event/death.hpp>
#include <core/event/remove_entity.hpp>

namespace core {

void DeathSystem::operator()() {
    for (auto [source, target] : ecs.get_events<DeathEvent>()) {
        if (!ecs.has_entity(target)) {
            continue;
        }
        if (ecs.has_component<AnimationFSMComponent>(target)) {
            auto& animation_fsm = ecs.get_component<AnimationFSMComponent>(target);
            animation_fsm.next_condition2values["is_dead"] = true;

            // TODO: optimize
            ecs.remove_components<TrackComponent, SpeedComponent, RigidbodyTag>(target);
            ColliderManager::instance().remove(target);
            if (ecs.has_component<ChildrenComponent>(target)) {
                for (auto child : ecs.get_component<ChildrenComponent>(target).children) {
                    if (ecs.has_component<AttackComponent>(child)) {
                        ecs.emplace_event<RemoveEntityEvent>(child);
                    }
                }
            }
        } else {
            ecs.emplace_event<RemoveEntityEvent>(target);
        }
    }

    // TODO: optimize(use tag?)
    for (auto [entity, animation_fsm] : ecs.get_entity_and_components<AnimationFSMComponent>()) {
        if (animation_fsm.current_state == "end") {
            ecs.emplace_event<RemoveEntityEvent>(entity);
        }
    }
}

}  // namespace core
