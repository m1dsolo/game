#include <game/animation.hpp>

#include <algorithm>
#include <filesystem>
#include <format>

#include <game/global.hpp>

namespace wheel {

Animation::Animation(std::string_view path) {
    if (std::filesystem::is_directory(path)) {
        textures_ = read_textures(path);
    }
}

Animation::~Animation() {
    for (auto texture : textures_) {
        SDL_DestroyTexture(texture);
    }
}

SDL_Texture* Animation::get_texture(int idx) {
    return textures_[idx];
}

std::vector<SDL_Texture*> Animation::read_textures(std::string_view path) {
    std::vector<std::filesystem::directory_entry> entries(std::filesystem::directory_iterator{path}, {});
    std::sort(entries.begin(), entries.end());

    std::vector<SDL_Texture*> res(entries.size());
    std::transform(entries.begin(), entries.end(), res.begin(), [this](const auto& entry){
        return IMG_LoadTexture(renderer, entry.path().c_str());
    });

    return res;
}

Animations::Animations(std::string_view name) {
    const std::string& path = std::format("{}/resources/animation/{}", game_resource.path, name);
    animations_[0] = std::make_shared<Animation>(std::format("{}/normal/up", path));
    animations_[1] = std::make_shared<Animation>(std::format("{}/normal/down", path));
    animations_[2] = std::make_shared<Animation>(std::format("{}/normal/left", path));
    animations_[3] = std::make_shared<Animation>(std::format("{}/normal/right", path));
    sketch_animations_[0] = std::make_shared<Animation>(std::format("{}/sketch/up", path));
    sketch_animations_[1] = std::make_shared<Animation>(std::format("{}/sketch/down", path));
    sketch_animations_[2] = std::make_shared<Animation>(std::format("{}/sketch/left", path));
    sketch_animations_[3] = std::make_shared<Animation>(std::format("{}/sketch/right", path));

    frames_ = animations_[1]->frames();
}

SDL_Texture* Animations::get_texture(int idx, int orient, bool sketch) {
    if (sketch) {
        if (sketch_animations_[orient]) {
            return sketch_animations_[orient]->get_texture(idx);
        } else {
            return sketch_animations_[1]->get_texture(idx);
        }
    } else {
        if (animations_[orient]) {
            return animations_[orient]->get_texture(idx);
        } else {
            return animations_[1]->get_texture(idx);
        }

    }
}

}  // namespace wheel
