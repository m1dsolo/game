#pragma once

#include <sdl/sdl.hpp>

namespace core {

struct ButtonComponent {
    SDL_FColor normal_color{ 230.f / 255.f, 230.f / 255.f, 230.f / 255.f, 1.f };
    SDL_FColor hovered_color{ 150.f / 255.f, 190.f / 255.f, 250.f / 255.f, 1.f };
    SDL_FColor pressed_color{ 200.f / 255.f, 160.f / 255.f, 250.f / 255.f, 1.f };
};

}  // namespace core
