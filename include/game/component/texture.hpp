#pragma once

#include <string>

#include <SDL3/SDL.h>

#include <game/texture_manager.hpp>

namespace wheel {

struct TextureComponent {
    TextureComponent(SDL_Texture* texture = nullptr) : texture(texture) {}
    TextureComponent(const std::string& name) : texture(TextureManager::instance().get_texture(name)) {}

    SDL_Texture* texture = nullptr;
};

}  // namespace wheel
