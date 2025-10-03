#pragma once

#include <sdl/sdl.hpp>

#include <functional>

namespace core {

struct ButtonComponent {
    std::function<void()> callback;

    SDL_FColor normal_color = {230.f / 255.f, 230.f / 255.f, 230.f / 255.f, 1.f};
    SDL_FColor hover_color = {150.f / 255.f, 190.f / 255.f, 250.f / 255.f, 1.f};
    SDL_FColor click_color = {200.f / 255.f, 160.f / 255.f, 250.f / 255.f, 1.f};

    enum class State {
        NORMAL,
        HOVER,
        CLICK
    } state{State::NORMAL};
};

}  // namespace core
