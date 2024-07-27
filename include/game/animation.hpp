#pragma once

#include <vector>
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

private:
    std::vector<SDL_Texture*> read_textures(std::string_view path);

    std::vector<SDL_Texture*> textures_;
};

class Animations {
public:
    Animations(std::string_view name);
    ~Animations() = default;

    SDL_Texture* get_texture(int idx, int orient, bool sketch = false);

    int frames() const { return frames_; }

private:
    int frames_ = 0;
    // up, down, left, right
    std::shared_ptr<Animation> animations_[4];
    std::shared_ptr<Animation> sketch_animations_[4];
};

}  // namespace wheel
