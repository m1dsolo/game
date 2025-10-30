#include <core/system/handle_sdl_event.hpp>
#include <core/manager/layer.hpp>

namespace core {

void HandleSDLEventSystem::operator()(wheel::ECS& ecs) {
    SDL_Event event;
    auto& layer_manager = LayerManager::instance();
    while (SDL_PollEvent(&event)) {
        layer_manager.handle_event(event);
    }
}

}  // namespace core
