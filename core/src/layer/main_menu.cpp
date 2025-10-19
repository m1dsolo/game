#include <core/layer/main_menu.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/sprite.hpp>
#include <core/manager/layer.hpp>
#include <core/layer/game.hpp>
#include <core/util/ui.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/text.hpp>
#include <core/component/button.hpp>
#include <core/component/sprite.hpp>
#include <core/component/render.hpp>
#include <core/component/layout.hpp>
#include <core/tag/render.hpp>
#include <core/resource/context.hpp>
#include <core/entity_event/button.hpp>
#include <core/entity_event/remove_entity.hpp>

namespace core {

void MainMenuLayer::on_attach() {
    const auto& context = ecs.get_resource<ContextResource>();
    auto& entity_manager = EntityManager::instance();
    start_button_entity_ = entity_manager.add_entity(
        NameComponent{"start_button"},
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
    auto start_text = entity_manager.add_entity(
        start_button_entity_,
        NameComponent{"start_text"},
        TextComponent{"start", 32, sdl::SDL::BLACK},
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
        NameComponent{"main_menu_layout"},
        LayoutComponent{{{start_button_entity_}, {exit_button_entity_}}}
    );

    entities_ = {
        start_button_entity_,
        start_text,
        exit_button_entity_,
        exit_text,
    };
}

void MainMenuLayer::on_detach() {
    for (auto entity : entities_) {
        ecs.add_component(entity, RemoveEntityEvent{});
    }
    ecs.add_component(layout_entity_, RemoveEntityEvent{});
}

void MainMenuLayer::on_show() {
    for (auto entity : entities_) {
        ecs.add_component(entity, RenderTag{});
    }
}

void MainMenuLayer::on_hide() {
    for (auto entity : entities_) {
        ecs.remove_component<RenderTag>(entity);
    }
}

void MainMenuLayer::on_update() {
    if (ecs.has_components<ButtonPressedEvent>(start_button_entity_)) {
        LayerManager::instance().push<GameLayer>();
    }

    if (ecs.has_component<ButtonPressedEvent>(exit_button_entity_)) {
        ecs.get_resource<ContextResource>().running = false;
    }
}

bool MainMenuLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_ESCAPE: {
                    ecs.get_resource<ContextResource>().running = false;
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
