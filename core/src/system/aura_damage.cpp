#include <core/system/aura_damage.hpp>
#include <core/global.hpp>
#include <core/component/aura_damage.hpp>
#include <core/component/hp.hpp>
#include <core/component/master.hpp>
#include <core/event/hp_change.hpp>
#include <core/event/trigger.hpp>

namespace core {

void AuraDamageSystem::operator()(wheel::ECS& ecs) {
    for (const auto [trigger, target] : ecs.get_events<TriggerEnterEvent>()) {
        if (ecs.has_component<AuraDamageComponent>(trigger) && ecs.has_component<HPComponent>(target)) {
            if (timer_id_mp_.contains({trigger, target})) {
                continue;
            }
            const auto& aura = ecs.get_component<AuraDamageComponent>(trigger);
            auto timer_id = timer.add(aura.interval, [trigger, target, &aura, &ecs]() {
                if (ecs.has_entity(trigger) && ecs.has_entity(target)) {
                    auto master = ecs.get_component<MasterComponent>(trigger).entity;
                    ecs.emplace_event<HPChangeEvent>(master, target, -aura.damage);
                    return aura.interval;
                }
                return 0;
            });
            timer_id_mp_[{trigger, target}] = timer_id;
        }
    }

    for (const auto [trigger, target] : ecs.get_events<TriggerExitEvent>()) {
        if (ecs.has_component<AuraDamageComponent>(trigger) && ecs.has_component<HPComponent>(target)) {
            if (auto it = timer_id_mp_.find({trigger, target}); it != timer_id_mp_.end()) {
                timer.remove(it->second);
                timer_id_mp_.erase(it);
            }
        }
    }
};

}  // namespace core
