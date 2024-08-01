#pragma once

#include <string>
#include <unordered_map>

#include <SDL3/SDL.h>

#include <wheel/singleton.hpp>

namespace wheel {

class TextureManager : public Singleton<TextureManager> {
    friend class Singleton<TextureManager>;

public:
    SDL_Texture* get_texture(const std::string& name, int w = 48, int h = 48);

private:
    TextureManager();
    ~TextureManager();
    TextureManager(const TextureManager&) = delete;

    std::unordered_map<std::string, SDL_Texture*> texture_map_;
};

}  // namespace wheel
