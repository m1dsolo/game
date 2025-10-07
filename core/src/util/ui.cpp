#include <core/util/ui.hpp>
#include <core/global.hpp>
#include <core/component/layout.hpp>
#include <core/entity_event/button.hpp>

#include <sdl/sdl.hpp>

namespace core {

bool UI::handle_layout_event(wheel::Entity entity, const SDL_Event& event) {
    // TODO: only support handling button for now
    auto& layout = ecs.get_component<LayoutComponent>(entity);
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_W: {
                    if (layout.selected.first > 0) {
                        auto selected_entity = layout.widgets[layout.selected.first][layout.selected.second];
                        ecs.add_entity_event(selected_entity, ButtonUnhoveredEvent{});
                        selected_entity = layout.widgets[--layout.selected.first][layout.selected.second];
                        ecs.add_entity_event(selected_entity, ButtonHoveredEvent{});
                        return true;
                    }
                    break;
                }
                case SDLK_S: {
                    if (layout.selected.first + 1 < layout.widgets.size()) {
                        auto selected_entity = layout.widgets[layout.selected.first][layout.selected.second];
                        ecs.add_entity_event(selected_entity, ButtonUnhoveredEvent{});
                        selected_entity = layout.widgets[++layout.selected.first][layout.selected.second];
                        ecs.add_entity_event(selected_entity, ButtonHoveredEvent{});
                        return true;
                    }
                    break;
                }
                case SDLK_RETURN: {
                    auto selected_entity = layout.widgets[layout.selected.first][layout.selected.second];
                    ecs.add_entity_event(selected_entity, ButtonPressedEvent{});
                    return true;
                }
            }
        }
        case SDL_EVENT_KEY_UP: {
            switch (event.key.key) {
                case SDLK_RETURN: {
                    auto selected_entity = layout.widgets[layout.selected.first][layout.selected.second];
                    ecs.add_entity_event(selected_entity, ButtonReleasedEvent{});
                    return true;
                }
            }
        }
    }
    return false;
}

}  // namespace core
