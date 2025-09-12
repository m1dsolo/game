#pragma once

#include <core/global.hpp>
#include <core/util/hierarchy.hpp>
#include <core/manager/collider.hpp>
#include <core/component/transform.hpp>
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

        // update transform
        // TODO: encapsulation
        auto& transform = ecs.get_component<TransformComponent>(entity);
        const auto& parent_transform = ecs.get_component<TransformComponent>(parent);
        transform.global.position = transform.local.position + parent_transform.global.position;
        transform.global.size = transform.local.size * parent_transform.global.scale;
        transform.global.scale = transform.local.scale * parent_transform.global.scale;
        transform.global.angle = transform.local.angle + parent_transform.global.angle;

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
