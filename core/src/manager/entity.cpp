#include <core/manager/entity.hpp>
#include <core/manager/texture.hpp>
#include <core/manager/sprite.hpp>
#include <core/manager/collider.hpp>
#include <core/manager/render.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/sprite.hpp>
#include <core/component/animation.hpp>
#include <core/component/children.hpp>
#include <core/component/parent.hpp>
#include <core/component/collider.hpp>
#include <core/component/button.hpp>
#include <core/component/layout.hpp>
#include <core/component/text.hpp>
#include <core/component/render.hpp>
#include <core/component/item.hpp>
#include <core/tag/root.hpp>
#include <core/tag/camera.hpp>
#include <core/entity_event/button.hpp>

namespace core {

EntityManager::EntityManager() {
    ecs.add_entity(
        NameComponent{"root"},
        TransformComponent{},
        ChildrenComponent{},
        RootTag{}
    );

    add_entity(
        NameComponent{"camera"},
        TransformComponent{{0.f, 0.f}, {1920.f, 1080.f}},
        CameraTag{}
    );
}

void EntityManager::remove_entity(wheel::Entity entity) {
    if (!ecs.has_entity(entity)) {
        return;
    }
    if (ecs.has_component<ChildrenComponent>(entity)) {
        for (auto child : ecs.get_component<ChildrenComponent>(entity).entities) {
            remove_entity(child);
        }
    }
    Hierarchy::detach_entity_from_parent(entity);
    ColliderManager::instance().remove(entity);
    if (ecs.has_component<RenderComponent>(entity)) {
        RenderManager::instance().remove(entity);
    }
    ecs.remove_entity(entity);
}

void EntityManager::update_text(wheel::Entity entity, const std::string& text) {
    auto& t = ecs.get_component<TextComponent>(entity);
    if (t.text != text) {
        if (t.text.length() > 0) {
            SpriteManager::instance().del(t.text);
        }
        update_text_(entity, text);
    }
}

void EntityManager::postprocess_entity_(wheel::Entity entity) {
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
            } else {
                const auto& name = ecs.get_component<NameComponent>(entity).name;
                auto texture = sdl::SDL::create_texture(name, 32, sdl::SDL::BLACK);
                sprite.sprite = &SpriteManager::instance().set(name, {texture});
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

        auto parent = ecs.get_component<ParentComponent>(entity).entity;
        const auto& parent_transform = ecs.get_component<TransformComponent>(parent);
        transform.global.position = transform.local.position + parent_transform.global.position;
        transform.global.size = transform.local.size * parent_transform.global.scale;
        transform.global.scale = transform.local.scale * parent_transform.global.scale;
        transform.global.angle = transform.local.angle + parent_transform.global.angle;
        if (parent_transform.type == Coordinate::Type::Screen) {
            transform.type = Coordinate::Type::Screen;
        }
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
}

void EntityManager::update_text_(wheel::Entity entity, const std::string& text) {
    auto& t = ecs.get_component<TextComponent>(entity);
    auto& sprite = ecs.get_component<SpriteComponent>(entity);
    auto& transform = ecs.get_component<TransformComponent>(entity);

    auto texture = sdl::SDL::create_texture(text, t.font_size, t.color);
    auto [w, h] = sdl::SDL::get_texture_size(texture);
    transform.local.size = transform.global.size = {w, h};
    transform.type = Coordinate::Type::Screen;
    sprite.sprite = &SpriteManager::instance().set(text, {texture});
    t.text = std::move(text);
}

void EntityManager::add_child_(wheel::Entity parent, wheel::Entity child) {
    if (!ecs.has_component<ChildrenComponent>(parent)) {
        ecs.add_component(parent, ChildrenComponent{});
    }
    ecs.get_component<ChildrenComponent>(parent).entities.emplace_back(child);
    ecs.add_component(child, ParentComponent{parent});
}

}  // namespace core
