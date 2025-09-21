#pragma once

#include <core/sprite.hpp>
#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>

#include <string>
#include <unordered_map>

#include <iostream>

namespace core {

class SpriteManager : public wheel::Singleton<SpriteManager> {
    friend wheel::Singleton<SpriteManager>;

public:
    const Sprite& set(const std::string& name, const Sprite& sprite) {
        std::cout << "sprite_set: " << name << " " << sprite.texture << " " << sprite.rect.x << " " << sprite.rect.y << " " << sprite.rect.w << " " << sprite.rect.h << std::endl;
        return name2sprites_[name] = sprite;
    }

    const Sprite& get(const std::string& name) const {
        if (name2sprites_.find(name) == name2sprites_.end()) {
            std::cout << "not found sprite: " << name << "!!!!!" << std::endl;
            return name2sprites_.at("");
        }
        // std::cout << "sprite_get: " << name << std::endl;
        return name2sprites_.at(name);
    }

    bool has(const std::string& name) const {
        return name2sprites_.find(name) != name2sprites_.end();
    }

    void del(const std::string& name);

    const Sprite& get(SDL_FColor color) {
        auto key = std::to_string(color.r) + "_" + std::to_string(color.g) + "_" + std::to_string(color.b) + "_" + std::to_string(color.a);
        if (!has(key)) {
            set(key, Sprite{
                sdl::SDL::create_texture(1.f, 1.f, color),
                {0.f, 0.f, 1.f, 1.f}
            });
        }
        return get(key);
    }

private:
    SpriteManager();
    SpriteManager(const SpriteManager&) = delete;

    std::unordered_map<std::string, Sprite> name2sprites_;
};

}  // namespace core
