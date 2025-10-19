#pragma once

#include <core/global.hpp>
#include <core/util/hierarchy.hpp>

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
        postprocess_entity_(entity);
        return entity;
    }

    void remove_entity(wheel::Entity entity);

    void update_text(wheel::Entity entity, const std::string& text);

private:
    EntityManager();
    EntityManager(const EntityManager&) = delete;

    void postprocess_entity_(wheel::Entity entity);
    void add_child_(wheel::Entity parent, wheel::Entity child);
    void update_text_(wheel::Entity entity, const std::string& text);

    std::function<void(wheel::Entity)> add_entity_callback_;
};

}  // namespace core
