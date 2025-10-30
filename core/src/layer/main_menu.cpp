#include <core/layer/main_menu.hpp>
#include <core/global.hpp>
#include <core/layer/game.hpp>
#include <core/util/ui.hpp>
#include <core/component/layout.hpp>
#include <core/component/children.hpp>
#include <core/tag/root.hpp>
#include <core/tag/camera.hpp>
#include <core/tag/render.hpp>
#include <core/tag/main_menu_layer.hpp>
#include <core/resource/context.hpp>
#include <core/event/layer.hpp>
#include <core/entity_event/button.hpp>
#include <core/entity_event/remove_entity.hpp>

namespace core {

void MainMenuLayer::on_attach() {
    layout_entity_ = UI::add_vertical_button_layout(
        "main_menu",
        {"new", "load", "exit"},
        200.f,
        100.f,
        20.f
    );
    auto& widgets = ecs.get_component<LayoutComponent>(layout_entity_).widgets;

    new_button_entity_ = widgets[0][0];
    load_button_entity_ = widgets[1][0];
    exit_button_entity_ = widgets[2][0];

    ecs.add_component(layout_entity_, MainMenuLayerTag{});
    for (auto& entities : widgets) {
        for (auto entity : entities) {
            ecs.add_component(entity, MainMenuLayerTag{});
            if (ecs.has_component<ChildrenComponent>(entity)) {
                for (auto child : ecs.get_component<ChildrenComponent>(entity).entities) {
                    ecs.add_component(child, MainMenuLayerTag{});
                }
            }
        }
    }
}

void MainMenuLayer::on_detach() {
    for (auto entity : ecs.get_entities<MainMenuLayerTag>()) {
        ecs.add_component(entity, RemoveEntityEvent{});
    }
}

void MainMenuLayer::on_show() {
    for (auto entity : ecs.get_entities<MainMenuLayerTag>()) {
        ecs.add_component(entity, RenderTag{});
    }

    auto& layout = ecs.get_component<LayoutComponent>(layout_entity_);
    auto selected_entity = layout.widgets[layout.selected.first][layout.selected.second];
    ecs.add_entity_event(selected_entity, ButtonUnhoveredEvent{});
    layout.selected = {0, 0};
    ecs.add_entity_event(layout.widgets[0][0], ButtonHoveredEvent{});
}

void MainMenuLayer::on_hide() {
    for (auto entity : ecs.get_entities<MainMenuLayerTag>()) {
        ecs.remove_component<RenderTag>(entity);
    }
}

void MainMenuLayer::on_update() {
    if (ecs.has_components<ButtonPressedEvent>(new_button_entity_)) {
        GameLayer::new_game_requested = true;
        ecs.emplace_event<AddLayerEvent>("GameLayer");
    }

    if (ecs.has_components<ButtonPressedEvent>(load_button_entity_)) {
        GameLayer::new_game_requested = false;
        ecs.emplace_event<AddLayerEvent>("GameLayer");
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
