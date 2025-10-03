#include <core/system/aura_damage.hpp>
#include <core/component/aura_damage.hpp>
#include <core/component/hp.hpp>
#include <core/component/master.hpp>
#include <core/event/hp_change.hpp>

#include <core/global.hpp>
#include <core/manager/time.hpp>
#include <core/event/trigger.hpp>

using namespace core;

namespace core {

void AuraDamageSystem::operator()() {
    for (const auto [trigger, target] : ecs.get_events<TriggerEnterEvent>()) {
        if (ecs.has_component<AuraDamageComponent>(trigger) && ecs.has_component<HPComponent>(target)) {
            const auto& aura = ecs.get_component<AuraDamageComponent>(trigger);
            auto timer_id = TimeManager::instance().timer().add(aura.interval, [trigger, target, &aura]() {
                auto master = ecs.get_component<MasterComponent>(trigger).entity;
                ecs.emplace_event<HPChangeEvent>(master, target, -aura.damage);
                return aura.interval;
            });
            timer_id_mp_[{trigger, target}] = timer_id;
        }
    }

    for (const auto [trigger, target] : ecs.get_events<TriggerExitEvent>()) {
        if (ecs.has_component<AuraDamageComponent>(trigger) && ecs.has_component<HPComponent>(target)) {
            if (auto it = timer_id_mp_.find({trigger, target}); it != timer_id_mp_.end()) {
                TimeManager::instance().timer().remove(it->second);
                timer_id_mp_.erase(it);
            }
        }
    }
};

}  // namespace core
