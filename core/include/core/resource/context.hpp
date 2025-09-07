#pragma once

#include <filesystem>

struct SDL_Texture;

namespace core {

struct ContextResource {
    bool running = true;
    std::filesystem::path exe_dir;
    SDL_Texture* texture = nullptr;
};

}  // namespace core
