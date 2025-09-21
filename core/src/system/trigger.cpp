#include <core/system/trigger.hpp>
#include <core/global.hpp>
#include <core/manager/collider.hpp>
#include <core/component/trigger.hpp>

namespace core {

void TriggerSystem::operator()() {
    for (auto [entity, trigger] : ecs.get_entity_and_components<TriggerComponent>()) {
        auto current_entities = ColliderManager::instance().query(entity) | 
            std::ranges::to<std::unordered_set<wheel::Entity>>();

        for (auto other : current_entities) {
            if (trigger.stay_entities.contains(other)) {
                if (trigger.on_stay) {
                    trigger.on_stay(entity, other);
                }
            } else {
                if (trigger.on_enter) {
                    trigger.on_enter(entity, other);
                }
            }
        }

        auto exited_entities = trigger.stay_entities | 
            std::views::filter([&](wheel::Entity e) {
                return current_entities.find(e) == current_entities.end();
            });
        for (auto other : exited_entities) {
            if (trigger.on_exit) {
                trigger.on_exit(entity, other);
            }
        }

        trigger.stay_entities = std::move(current_entities);
    }
}

}  // namespace core
