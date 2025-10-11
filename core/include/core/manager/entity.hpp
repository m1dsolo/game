#pragma once

#include <core/global.hpp>
#include <core/manager/texture.hpp>
#include <core/manager/sprite.hpp>
#include <core/manager/collider.hpp>
#include <core/manager/render.hpp>
#include <core/util/hierarchy.hpp>
#include <core/component/transform.hpp>
#include <core/component/sprite.hpp>
#include <core/component/animation.hpp>
#include <core/component/children.hpp>
#include <core/component/collider.hpp>
#include <core/component/button.hpp>
#include <core/component/layout.hpp>
#include <core/component/text.hpp>
#include <core/component/render.hpp>
#include <core/entity_event/button.hpp>

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

        if (ecs.has_component<ColliderComponent>(entity)) {
            if (!ecs.has_component<TransformComponent>(entity)) {
                wheel::Log::error("Entity with ColliderComponent must have TransformComponent");
            }
            ColliderManager::instance().add(entity);
        }

        if (ecs.has_component<RenderComponent>(entity)) {
            RenderManager::instance().add(entity);
        }

        if (ecs.has_component<RenderComponent>(entity)) {
            auto& sprite = ecs.get_component<SpriteComponent>(entity);
            if (sprite.sprite->texture == TextureManager::instance().get("")) {
                if (ecs.has_component<AnimationComponent>(entity)) {
                    const auto& animation = *ecs.get_component<AnimationComponent>(entity).animation;
                    sprite.sprite = &animation.sprites[0];
                }
            }
        }

        // update transform
        // TODO: encapsulation
        if (ecs.has_component<TransformComponent>(entity)) {
            auto& transform = ecs.get_component<TransformComponent>(entity);
            if (transform.local.size == 0.f) {
                if (ecs.has_component<SpriteComponent>(entity)) {
                    const auto& sprite = ecs.get_component<SpriteComponent>(entity);
                    transform.local.size = {sprite.sprite->rect.w, sprite.sprite->rect.h};
                }
            }

            const auto& parent_transform = ecs.get_component<TransformComponent>(parent);
            transform.global.position = transform.local.position + parent_transform.global.position;
            transform.global.size = transform.local.size * parent_transform.global.scale;
            transform.global.scale = transform.local.scale * parent_transform.global.scale;
            transform.global.angle = transform.local.angle + parent_transform.global.angle;
        }

        if (ecs.has_component<ButtonComponent>(entity)) {
            auto& button = ecs.get_component<ButtonComponent>(entity);
            auto& sprite = ecs.get_component<SpriteComponent>(entity);
            sprite.sprite = &SpriteManager::instance().get(button.normal_color);
        }

        if (ecs.has_component<LayoutComponent>(entity)) {
            auto& layout = ecs.get_component<LayoutComponent>(entity);
            ecs.add_entity_event(layout.widgets[layout.selected.first][layout.selected.second], ButtonHoveredEvent{});
        }

        if (ecs.has_component<TextComponent>(entity)) {
            update_text_(entity, ecs.get_component<TextComponent>(entity).text);
        }

        if (add_entity_callback_) {
            add_entity_callback_(entity);
        }

        return entity;
    }

    void remove_entity(wheel::Entity entity);

    void update_text(wheel::Entity entity, const std::string& text);

private:
    EntityManager();
    EntityManager(const EntityManager&) = delete;

    void add_child_(wheel::Entity parent, wheel::Entity child);
    void update_text_(wheel::Entity entity, const std::string& text);

    std::function<void(wheel::Entity)> add_entity_callback_;
};

}  // namespace core
