#pragma once

struct SDL_Texture;

namespace core {

struct ContextResource {
    bool running = true;
    SDL_Texture* texture = nullptr;
};

}  // namespace core
