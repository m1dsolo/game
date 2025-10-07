#include <core/system/attack.hpp>
#include <core/global.hpp>
#include <core/manager/time.hpp>
#include <core/component/transform.hpp>
#include <core/component/hp.hpp>
#include <core/component/attack.hpp>
#include <core/component/master.hpp>
#include <core/component/animation_fsm.hpp>
#include <core/resource/trigger.hpp>
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


    // attack the closest target
    const auto& closest_target = ecs.get_resource<TriggerResource>().closest_target;
    for (auto [trigger, target_pair] : closest_target) {
        if (!ecs.has_component<AttackComponent>(trigger)) {
            continue;
        }
        auto [target, distance] = target_pair;
        if (!ecs.has_entity(target)) {
            continue;
        }

        auto& attack = ecs.get_component<AttackComponent>(trigger);
        if (attack.time < attack.interval) {
            continue;
        }
        attack.time -= attack.interval;

        auto master = ecs.get_component<MasterComponent>(trigger).entity;
        if (ecs.has_component<AnimationFSMComponent>(master)) {
            auto& fsm = ecs.get_component<AnimationFSMComponent>(master);
            fsm.next_condition2values["is_attacking"] = true;
        }

        ecs.emplace_event<HPChangeEvent>(master, target, -attack.damage);
    }
}

}  // namespace core
