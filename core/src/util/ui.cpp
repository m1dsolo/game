#include <core/util/ui.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/text.hpp>
#include <core/component/button.hpp>
#include <core/component/sprite.hpp>
#include <core/component/render.hpp>
#include <core/component/layout.hpp>
#include <core/resource/context.hpp>
#include <core/entity_event/button.hpp>

#include <sdl/sdl.hpp>

#include <format>
#include <ranges>

namespace core {

wheel::Entity UI::add_vertical_button_layout(
    std::string_view name,
    std::vector<std::string_view> texts,
    float button_width,
    float button_height,
    float spacing
) {
    const auto& context = ecs.get_resource<ContextResource>();
    auto& entity_manager = EntityManager::instance();

    int num = texts.size();
    float width = button_width + spacing * 2;
    float height = button_height * num + spacing * (num + 1);
    float top = -height / 2 + spacing + button_height / 2;

    auto layout_entity = entity_manager.add_entity(
        NameComponent{std::format("{}_layout", name)},
        TransformComponent{
            {{context.virtual_window_width / 2.f, context.virtual_window_height / 2.f}, {width, height}},
            {},
            Coordinate::Type::Screen
        },
        LayoutComponent{},
        SpriteComponent{std::format("{}_layout", name)},
        RenderComponent{20}
    );
    auto& layout = ecs.get_component<LayoutComponent>(layout_entity);

    for (const auto& [i, text] : std::views::enumerate(texts)) {
        auto button_entity = entity_manager.add_entity(
            layout_entity,
            NameComponent{std::format("{}_button", text)},
            ButtonComponent{},
            TransformComponent{{{0.f, top + i * (button_height + spacing)}, {button_width, button_height}}},
            SpriteComponent{},
            RenderComponent{21}
        );
        auto text_entity = entity_manager.add_entity(
            button_entity,
            NameComponent{std::format("{}_text", text)},
            TextComponent{std::string(text), 32, sdl::SDL::Color::Black},
            TransformComponent{},
            SpriteComponent{},
            RenderComponent{22}
        );
        layout.widgets.push_back({button_entity});
    }

    return layout_entity;
}

void select_layout_widget(LayoutComponent& layout, int delta_row, int delta_col) {
    int n = layout.widgets.size();
    int m = layout.widgets[0].size();
    int new_row = (layout.selected.first + delta_row + n) % n;
    int new_col = (layout.selected.second + delta_col + m) % m;
    auto selected_entity = layout.widgets[layout.selected.first][layout.selected.second];
    ecs.add_entity_event(selected_entity, ButtonUnhoveredEvent{});
    ecs.get_component<ButtonComponent>(selected_entity).is_hovered = false;
    layout.selected = {new_row, new_col};
    selected_entity = layout.widgets[layout.selected.first][layout.selected.second];
    ecs.add_entity_event(selected_entity, ButtonHoveredEvent{});
    ecs.get_component<ButtonComponent>(selected_entity).is_hovered = true;
}

bool UI::handle_layout_event(wheel::Entity entity, const SDL_Event& event) {
    // TODO: only support handling button for now
    auto& layout = ecs.get_component<LayoutComponent>(entity);
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_W: {
                    select_layout_widget(layout, -1, 0);
                    return true;
                }
                case SDLK_S: {
                    select_layout_widget(layout, 1, 0);
                    break;
                }
                case SDLK_RETURN: {
                    auto selected_entity = layout.widgets[layout.selected.first][layout.selected.second];
                    ecs.add_entity_event(selected_entity, ButtonPressedEvent{});
                    layout.pressed = true;
                    return true;
                }
            }
        }
        case SDL_EVENT_KEY_UP: {
            switch (event.key.key) {
                case SDLK_RETURN: {
                    if (layout.pressed) {
                        auto selected_entity = layout.widgets[layout.selected.first][layout.selected.second];
                        ecs.add_entity_event(selected_entity, ButtonReleasedEvent{});
                        layout.pressed = false;
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

}  // namespace core
