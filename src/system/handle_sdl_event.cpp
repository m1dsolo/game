#include <game/system/handle_sdl_event.hpp>

#include <game/ui.hpp>

namespace wheel {

void HandleSDLEventSystem::execute_impl() {
    auto& ui = UI::instance();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ui.handle_event(event);
    }
}

}  // namespace wheel
