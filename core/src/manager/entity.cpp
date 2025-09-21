#include <core/manager/entity.hpp>
#include <core/manager/sprite.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/children.hpp>
#include <core/component/sprite.hpp>
#include <core/component/text.hpp>
#include <core/tag/root.hpp>
#include <core/tag/camera.hpp>

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

void EntityManager::del_entity(wheel::Entity entity) {
    if (ecs.has_component<ChildrenComponent>(entity)) {
        for (auto child : ecs.get_component<ChildrenComponent>(entity).children) {
            del_entity(child);
        }
    }
    Hierarchy::detach_entity_from_parent(entity);
    ColliderManager::instance().remove(entity);
    ecs.del_entity(entity);
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

void EntityManager::update_text_(wheel::Entity entity, const std::string& text) {
    auto& t = ecs.get_component<TextComponent>(entity);
    auto& sprite = ecs.get_component<SpriteComponent>(entity);
    auto& transform = ecs.get_component<TransformComponent>(entity);

    auto texture = sdl::SDL::create_texture(text, t.font_size, t.color);
    auto [w, h] = sdl::SDL::get_texture_size(texture);
    transform.local.size = transform.global.size = {w, h};
    transform.type = Coordinate::Type::SCREEN;
    sprite.sprite = &SpriteManager::instance().set(text, {texture});
    t.text = std::move(text);
}

void EntityManager::add_child_(wheel::Entity parent, wheel::Entity child) {
    if (!ecs.has_component<ChildrenComponent>(parent)) {
        ecs.add_component(parent, ChildrenComponent{});
    }
    ecs.get_component<ChildrenComponent>(parent).children.emplace_back(child);
    ecs.add_component(child, ParentComponent{parent});
}

}  // namespace core
