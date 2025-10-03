#include <core/system/death.hpp>
#include <core/global.hpp>
#include <core/component/animation_fsm.hpp>
#include <core/component/hp.hpp>
#include <core/component/speed.hpp>
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
            animation_fsm.condition2values["is_dead"] = true;
            ecs.remove_components<HPComponent, SpeedComponent>(target);
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
