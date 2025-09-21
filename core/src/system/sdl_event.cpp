#include <core/system/sdl_event.hpp>
#include <core/manager/ui.hpp>

namespace core {

void SDLEventSystem::operator()() {
    while (SDL_PollEvent(&event_)) {
        UIManager::instance().handle_event(event_);
    }
}

}  // namespace core
