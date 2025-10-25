#pragma once

#include <core/component/sprite.hpp>

#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>
#include <wheel/id.hpp>

#include <unordered_map>
#include <format>

#include <iostream>

namespace core {

class SpriteManager : public wheel::Singleton<SpriteManager> {
    friend wheel::Singleton<SpriteManager>;

public:
    const Sprite& set(wheel::ID id, const Sprite& sprite) {
        // std::cout << "sprite_set: " << name << " " << sprite.texture << " " << sprite.rect.x << " " << sprite.rect.y << " " << sprite.rect.w << " " << sprite.rect.h << std::endl;
        return id2sprites_[id] = sprite;
    }

    const Sprite& get(wheel::ID id) const {
        if (id2sprites_.find(id) == id2sprites_.end()) {
            std::cout << "not found sprite: " << id << "!!!!!" << std::endl;
            return id2sprites_.at("");
        }
        // std::cout << "sprite_get: " << name << std::endl;
        return id2sprites_.at(id);
    }

    bool has(wheel::ID id) const {
        return id2sprites_.find(id) != id2sprites_.end();
    }

    void del(wheel::ID id);

    const Sprite& get(SDL_FColor color) {
        wheel::ID id = std::format("{}_{}_{}_{}", color.r, color.g, color.b, color.a);
        if (!has(id)) {
            set(id, Sprite{
                sdl::SDL::create_texture(1.f, 1.f, color),
                {0.f, 0.f, 1.f, 1.f}
            });
        }
        return get(id);
    }

private:
    SpriteManager();
    SpriteManager(const SpriteManager&) = delete;

    std::unordered_map<wheel::ID, Sprite> id2sprites_;
};

}  // namespace core
