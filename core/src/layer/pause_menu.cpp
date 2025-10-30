#include <core/layer/pause_menu.hpp>
#include <core/layer/game.hpp>
#include <core/global.hpp>
#include <core/manager/save.hpp>
#include <core/manager/layer.hpp>
#include <core/util/ui.hpp>
#include <core/component/layout.hpp>
#include <core/tag/render.hpp>
#include <core/tag/pause_menu_layer.hpp>
#include <core/tag/root.hpp>
#include <core/resource/context.hpp>
#include <core/event/layer.hpp>
#include <core/entity_event/button.hpp>
#include <core/entity_event/remove_entity.hpp>
#include <core/entity_event/button.hpp>

namespace core {

void PauseMenuLayer::on_attach() {
    layout_entity_ = UI::add_vertical_button_layout(
        "pause_menu",
        {"resume", "save", "load", "menu", "exit"},
        200.f,
        100.f,
        20.f
    );
    auto& widgets = ecs.get_component<LayoutComponent>(layout_entity_).widgets;

    resume_button_entity_ = widgets[0][0];
    save_button_entity_ = widgets[1][0];
    load_button_entity_ = widgets[2][0];
    menu_button_entity_ = widgets[3][0];
    exit_button_entity_ = widgets[4][0];

    ecs.add_component(layout_entity_, PauseMenuLayerTag{});
    for (auto& entities : widgets) {
        for (auto entity : entities) {
            ecs.add_component(entity, PauseMenuLayerTag{});
            if (ecs.has_component<ChildrenComponent>(entity)) {
                for (auto child : ecs.get_component<ChildrenComponent>(entity).entities) {
                    ecs.add_component(child, PauseMenuLayerTag{});
                }
            }
        }
    }
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
    ecs.add_entity_event(resume_button_entity_, ButtonHoveredEvent{});
}

void PauseMenuLayer::on_hide() {
    for (auto entity : ecs.get_entities<PauseMenuLayerTag>()) {
        ecs.remove_component<RenderTag>(entity);
    }
    ecs.add_entity_event(resume_button_entity_, ButtonUnhoveredEvent{});
}

void PauseMenuLayer::on_update() {
    if (ecs.has_component<ButtonPressedEvent>(resume_button_entity_)) {
        ecs.emplace_event<RemoveLayerEvent>();
    }

    if (ecs.has_component<ButtonPressedEvent>(save_button_entity_)) {
        SaveManager::instance().save();
    }

    if (ecs.has_component<ButtonPressedEvent>(load_button_entity_)) {
        ecs.emplace_event<RemoveLayerEvent>();
        ecs.emplace_event<RemoveLayerEvent>();
        GameLayer::new_game_requested = false;
        ecs.emplace_event<AddLayerEvent>("GameLayer");
    }

    if (ecs.has_component<ButtonPressedEvent>(menu_button_entity_)) {
        ecs.emplace_event<RemoveLayerEvent>();
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
