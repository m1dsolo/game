#include <core/system/attack.hpp>
#include <core/global.hpp>
#include <core/manager/time.hpp>
#include <core/component/transform.hpp>
#include <core/component/hp.hpp>
#include <core/component/attack.hpp>
#include <core/component/master.hpp>
#include <core/component/animation_fsm.hpp>
#include <core/event/trigger.hpp>
#include <core/event/hp_change.hpp>

namespace core {

void AttackSystem::operator()() {
    auto dt = TimeManager::instance().dt();
    for (auto [attack] : ecs.get_components<AttackComponent>()) {
        if (attack.time < attack.interval) {
            attack.time += dt;
        } else {
            attack.time = attack.interval;
        }
    }

    // find targets in range
    std::unordered_map<wheel::Entity, std::vector<wheel::Entity>> in_range_mp;
    for (const auto [trigger, target] : ecs.get_events<TriggerStayEvent>()) {
        if (ecs.has_component<AttackComponent>(trigger) && ecs.has_component<HPComponent>(target)) {
            in_range_mp[trigger].emplace_back(target);
        }
    }

    // attack the closest target
    for (const auto& [trigger, targets] : in_range_mp) {
        auto& attack = ecs.get_component<AttackComponent>(trigger);
        if (attack.time < attack.interval) {
            continue;
        }
        attack.time -= attack.interval;

        const auto& trigger_pos = ecs.get_component<TransformComponent>(trigger).global.position;
        auto closest = *std::min_element(targets.begin(), targets.end(), [&](auto target0, auto target1) {
            const auto& target0_pos = ecs.get_component<TransformComponent>(target0).global.position;
            const auto& target1_pos = ecs.get_component<TransformComponent>(target1).global.position;
            return trigger_pos.euclidean_distance(target0_pos) < trigger_pos.euclidean_distance(target1_pos);
        });

        auto master = ecs.get_component<MasterComponent>(trigger).entity;
        if (ecs.has_component<AnimationFSMComponent>(master)) {
            auto& fsm = ecs.get_component<AnimationFSMComponent>(master);
            fsm.next_condition2values["is_attacking"] = true;
        }
        ecs.emplace_event<HPChangeEvent>(master, closest, -attack.damage);
    }
}

}  // namespace core
