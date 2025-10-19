#include <core/layer/pause_menu.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/sprite.hpp>
#include <core/util/ui.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/text.hpp>
#include <core/component/button.hpp>
#include <core/component/sprite.hpp>
#include <core/component/render.hpp>
#include <core/component/layout.hpp>
#include <core/tag/render.hpp>
#include <core/tag/layer.hpp>
#include <core/resource/context.hpp>
#include <core/entity_event/button.hpp>
#include <core/entity_event/remove_entity.hpp>
#include <core/entity_event/remove_layer.hpp>
#include <core/entity_event/button.hpp>

namespace core {

void PauseMenuLayer::on_attach() {
    const auto& context = ecs.get_resource<ContextResource>();
    auto& entity_manager = EntityManager::instance();

    layout_entity_ = entity_manager.add_entity(
        NameComponent{"pause_menu_layout"},
        TransformComponent{
            {context.virtual_window_width / 2.f, context.virtual_window_height / 2.f},
            {240.f, 260.f},
            {1.f, 1.f},
            Coordinate::Type::Screen
        },
        LayoutComponent{},
        SpriteComponent{sdl::SDL::Color::Gray},
        RenderComponent{2},
        PauseMenuLayerTag{}
    );

    resume_button_entity_ = entity_manager.add_entity(
        layout_entity_,
        NameComponent{"resume_button"},
        ButtonComponent{},
        TransformComponent{{0.f, -60.f}, {200.f, 100.f}},
        SpriteComponent{},
        RenderComponent{3},
        PauseMenuLayerTag{}
    );
    auto resume_text = entity_manager.add_entity(
        resume_button_entity_,
        NameComponent{"resume_text"},
        TextComponent{"resume", 32, sdl::SDL::Color::Black},
        TransformComponent{},
        SpriteComponent{},
        RenderComponent{4},
        PauseMenuLayerTag{}
    );

    exit_button_entity_ = entity_manager.add_entity(
        layout_entity_,
        NameComponent{"exit_button"},
        ButtonComponent{},
        TransformComponent{{0.f, 60.f}, {200.f, 100.f}},
        SpriteComponent{},
        RenderComponent{3},
        PauseMenuLayerTag{}
    );
    auto exit_text = entity_manager.add_entity(
        exit_button_entity_,
        NameComponent{"exit_text"},
        TextComponent{"exit", 32, sdl::SDL::Color::Black},
        TransformComponent{},
        SpriteComponent{},
        RenderComponent{4},
        PauseMenuLayerTag{}
    );

    auto& layout = ecs.get_component<LayoutComponent>(layout_entity_);
    layout.widgets = {{resume_button_entity_}, {exit_button_entity_}};
    ecs.add_entity_event(resume_button_entity_, ButtonHoveredEvent{});
}

void PauseMenuLayer::on_detach() {
    for (auto entity : ecs.get_entities<PauseMenuLayerTag>()) {
        ecs.add_component(entity, RemoveEntityEvent{});
    }
}

void PauseMenuLayer::on_show() {
    for (auto entity : ecs.get_entities<PauseMenuLayerTag>()) {
        ecs.add_component(entity, RenderTag{});
    }
}

void PauseMenuLayer::on_hide() {
    for (auto entity : ecs.get_entities<PauseMenuLayerTag>()) {
        ecs.remove_component<RenderTag>(entity);
    }
}

void PauseMenuLayer::on_update() {
    if (ecs.has_component<ButtonPressedEvent>(resume_button_entity_)) {
        ecs.emplace_event<RemoveLayerEvent>();
    }

    if (ecs.has_component<ButtonPressedEvent>(exit_button_entity_)) {
        ecs.get_resource<ContextResource>().running = false;
    }
}

bool PauseMenuLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_ESCAPE: {
                    ecs.emplace_event<RemoveLayerEvent>();
                    return true;
                }
            }
        }
    }
    if (UI::handle_layout_event(layout_entity_, event)) {
        return true;
    }
    return false;
}

}  // namespace core
