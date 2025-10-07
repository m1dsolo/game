#include <core/system/handle_sdl_event.hpp>
#include <core/manager/layer.hpp>

namespace core {

void HandleSDLEventSystem::operator()() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        LayerManager::instance().handle_event(event);
    }
}

}  // namespace core
