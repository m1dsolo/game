#pragma once

#include <core/global.hpp>
#include <core/util/hierarchy.hpp>
#include <core/manager/collider.hpp>
#include <core/component/children.hpp>
#include <core/component/parent.hpp>
#include <core/component/collider.hpp>
#include <core/component/trigger.hpp>
#include <wheel/singleton.hpp>

#include <functional>

namespace core {

class EntityManager : public wheel::Singleton<EntityManager> {
    friend wheel::Singleton<EntityManager>;

public:
    void set_add_entity_callback(std::function<void(wheel::Entity)> callback) {
        add_entity_callback_ = callback;
    }

    template <typename... ComponentTypes>
    wheel::Entity add_entity(ComponentTypes&&... components) {
        return add_entity(Hierarchy::root(), std::forward<ComponentTypes>(components)...);
    }

    template <typename... ComponentTypes>
    wheel::Entity add_entity(wheel::Entity parent, ComponentTypes&&... components) {
        auto entity = ecs.add_entity(std::forward<ComponentTypes>(components)...);

        add_child_(parent, entity);

        if (ecs.has_component<ColliderComponent>(entity) || ecs.has_component<TriggerComponent>(entity)) {
            ColliderManager::instance().add(entity);
        }

        if (add_entity_callback_) {
            add_entity_callback_(entity);
        }

        return entity;
    }

    void del_entity(wheel::Entity entity);

private:
    EntityManager();
    EntityManager(const EntityManager&) = delete;

    void add_child_(wheel::Entity parent, wheel::Entity child);

    std::function<void(wheel::Entity)> add_entity_callback_;
};

}  // namespace core
