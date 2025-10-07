#include <core/system/trigger.hpp>
#include <core/global.hpp>
#include <core/component/transform.hpp>
#include <core/resource/trigger.hpp>
#include <core/event/trigger.hpp>

namespace core {

void TriggerSystem::operator()() {
    auto& closest_target = ecs.get_resource<TriggerResource>().closest_target;
    closest_target.clear();

    for (const auto [trigger, target] : ecs.get_events<TriggerStayEvent>()) {
        if (!ecs.has_component<TransformComponent>(trigger) || !ecs.has_component<TransformComponent>(target)) {
            continue;
        }
        const auto& trigger_pos = ecs.get_component<TransformComponent>(trigger).global.position;
        const auto& target_pos = ecs.get_component<TransformComponent>(target).global.position;
        auto dist = trigger_pos.euclidean_distance(target_pos);
        if (!closest_target.count(trigger) || dist < closest_target[trigger].second) {
            closest_target[trigger] = {target, dist};
        }
    }
}

}  // namespace core
