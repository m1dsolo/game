#include <core/system/collider.hpp>
#include <core/manager/collider.hpp>
#include <core/component/collider.hpp>
#include <core/event/trigger.hpp>

namespace core {

void ColliderSystem::operator()() {
    auto& collider_manager = ColliderManager::instance();
    collider_manager.update();

    last_collided_entities_ = std::move(current_collided_entities_);

    for (auto [entity0, entity1] : collider_manager.query_all()) {
        helper_(entity0, entity1);
        helper_(entity1, entity0);
    }
    
    for (const auto& [trigger, current] : current_collided_entities_) {
        for (const auto entity : current) {
            if (last_collided_entities_[trigger].contains(entity)) {
                // stay : current & last
                ecs.emplace_event<TriggerStayEvent>(trigger, entity);
            } else {
                // enter : current - last
                ecs.emplace_event<TriggerEnterEvent>(trigger, entity);
            }
        }
    }
    for (const auto& [trigger, last] : last_collided_entities_) {
        for (const auto entity : last) {
            if (!current_collided_entities_[trigger].contains(entity)) {
                // exit : last - current
                ecs.emplace_event<TriggerExitEvent>(trigger, entity);
            }
        }
    }
}

void ColliderSystem::helper_(wheel::Entity entity0, wheel::Entity entity1) {
    const auto& collider0 = ecs.get_component<ColliderComponent>(entity0);
    const auto& collider1 = ecs.get_component<ColliderComponent>(entity1);
    if ((collider0.mask & collider1.layer) != collider1.layer) {
        return;
    }
    current_collided_entities_[entity0].emplace(entity1);
}

}  // namespace core
