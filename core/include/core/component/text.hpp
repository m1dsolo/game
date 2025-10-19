#pragma once

#include <sdl/sdl.hpp>

#include <string>

namespace core {

struct TextComponent {
    std::string text;
    float font_size = 32;
    SDL_FColor color = sdl::SDL::Color::Black;
};

}  // namespace core
