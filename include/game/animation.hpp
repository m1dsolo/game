#pragma once

#include <array>
#include <vector>
#include <unordered_map>
#include <memory>
#include <string_view>

#include <SDL3/SDL.h>

namespace wheel {

class Animation {
public:
    Animation(std::string_view path);
    ~Animation();

    SDL_Texture* get_texture(int idx);

    int frames() const { return textures_.size(); }
    bool empty() const { return textures_.empty(); }

private:
    std::vector<SDL_Texture*> read_textures(std::string_view path);

    std::vector<SDL_Texture*> textures_;
};

class Animations {
public:
    enum class Type {
        NORMAL,
        SKETCH,
        ATTACK
    };

    Animations(std::string_view name);
    ~Animations() = default;

    SDL_Texture* get_texture(int idx, int orient, Type type = Type::NORMAL);

    int frames() const { return frames_; }

private:
    static std::array<std::shared_ptr<Animation>, 4> read_animations(std::string_view path);

    int frames_ = 0;
    std::unordered_map<Type, std::array<std::shared_ptr<Animation>, 4>> type2animations_;
};

}  // namespace wheel
