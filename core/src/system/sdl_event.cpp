#include <core/system/sdl_event.hpp>
#include <core/manager/ui.hpp>
#include <core/global.hpp>

namespace core {

void SDLEventSystem::update_impl() {
    while (SDL_PollEvent(&event_)) {
        UIManager::instance().handle_event(event_);
        // ecs.emplace_event<SDL_Event>(event_);

        // TODO: move to UIManager
        if (event_.type == SDL_EVENT_QUIT) {
            context.running = false;
        }
        if (event_.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
            event_.window.windowID == SDL_GetWindowID(sdl::SDL::window())) {
            context.running = false;
        }
    }
}

}  // namespace core
