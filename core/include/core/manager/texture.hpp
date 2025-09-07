#pragma once

#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>

#include <filesystem>
#include <unordered_map>
#include <iostream>

class SDL_Texture;

namespace core {

class TextureManager : public wheel::Singleton<TextureManager> {
    friend wheel::Singleton<TextureManager>;

public:
    bool has(const std::filesystem::path& path) const { return path2textures_.find(path) != path2textures_.end(); }
    void set(const std::filesystem::path& path, SDL_Texture* texture) {
        std::cout << "texture_set: " << path << std::endl;
        path2textures_[path] = texture;
    }
    SDL_Texture* get(const std::filesystem::path& path) {
        // std::cout << "texture_get: " << path << std::endl;
        return path2textures_[path];
    }

private:
    TextureManager();
    TextureManager(const TextureManager&) = delete;

    std::unordered_map<std::filesystem::path, SDL_Texture*> path2textures_;
};

}  // namespace core
