#include <game/system/input.hpp>

namespace wheel {

void InputSystem::execute_impl() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ui_.handle_event(event);
    }
}

}  // namespace wheel
