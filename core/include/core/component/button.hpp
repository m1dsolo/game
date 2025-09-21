#pragma once

#include <functional>

#include <sdl/sdl.hpp>

namespace core {

struct ButtonComponent {
    std::function<void()> callback;

    SDL_Color normal_color = {230, 230, 230, 255};
    SDL_Color hover_color = {150, 190, 250, 255};
    SDL_Color click_color = {200, 160, 250, 255};

    enum class State {
        NORMAL,
        HOVER,
        CLICK
    } state{State::NORMAL};
};

}  // namespace core
