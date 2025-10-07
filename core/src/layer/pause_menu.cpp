#include <core/layer/pause_menu.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/sprite.hpp>
#include <core/util/ui.hpp>
#include <core/component/name.hpp>
#include <core/component/text.hpp>
#include <core/component/button.hpp>
#include <core/component/sprite.hpp>
#include <core/component/render.hpp>
#include <core/component/layout.hpp>
#include <core/tag/render.hpp>
#include <core/entity_event/button.hpp>
#include <core/entity_event/remove_entity.hpp>
#include <core/entity_event/remove_layer.hpp>

namespace core {

void PauseMenuLayer::on_attach() {
    auto& entity_manager = EntityManager::instance();
    resume_button_entity_ = entity_manager.add_entity(
        NameComponent{"resume_button"},
        ButtonComponent{},
        TransformComponent{
            {context.virtual_window_width / 2.f, context.virtual_window_height / 2.f - 50.f},
            {200.f, 80.f},
            {1.f, 1.f},
            Coordinate::Type::Screen
        },
        SpriteComponent{},
        RenderComponent{3}
    );
    auto resume_text = entity_manager.add_entity(
        resume_button_entity_,
        NameComponent{"resume_text"},
        TextComponent{"resume", 32, sdl::SDL::BLACK},
        TransformComponent{},
        SpriteComponent{},
        RenderComponent{4}
    );

    exit_button_entity_ = entity_manager.add_entity(
        NameComponent{"exit_button"},
        ButtonComponent{},
        TransformComponent{
            {context.virtual_window_width / 2.f, context.virtual_window_height / 2.f + 50.f},
            {200.f, 80.f},
            {1.f, 1.f},
            Coordinate::Type::Screen
        },
        SpriteComponent{},
        RenderComponent{3}
    );
    auto exit_text = entity_manager.add_entity(
        exit_button_entity_,
        NameComponent{"exit_text"},
        TextComponent{"exit", 32, sdl::SDL::BLACK},
        TransformComponent{},
        SpriteComponent{},
        RenderComponent{4}
    );

    layout_entity_ = entity_manager.add_entity(
        NameComponent{"pause_menu_layout"},
        LayoutComponent{{{resume_button_entity_}, {exit_button_entity_}}}
    );

    entities_ = {
        resume_button_entity_,
        resume_text,
        exit_button_entity_,
        exit_text,
    };
}

void PauseMenuLayer::on_detach() {
    for (auto entity : entities_) {
        ecs.add_component(entity, RemoveEntityEvent{});
    }
    ecs.add_component(layout_entity_, RemoveEntityEvent{});
}

void PauseMenuLayer::on_show() {
    for (auto entity : entities_) {
        ecs.add_component(entity, RenderTag{});
    }
}

void PauseMenuLayer::on_hide() {
    for (auto entity : entities_) {
        ecs.remove_component<RenderTag>(entity);
    }
}

void PauseMenuLayer::on_update() {
    if (ecs.has_component<ButtonPressedEvent>(resume_button_entity_)) {
        ecs.emplace_event<RemoveLayerEvent>();
    }

    if (ecs.has_component<ButtonPressedEvent>(exit_button_entity_)) {
        context.running = false;
    }
}

bool PauseMenuLayer::on_event(const SDL_Event& event) {
    if (UI::handle_layout_event(layout_entity_, event)) {
        return true;
    }
    return false;
}

}  // namespace core
