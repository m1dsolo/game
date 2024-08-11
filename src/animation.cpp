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
        return sdl.load_img(entry.path().c_str());
    });

    return res;
}

Animations::Animations(std::string_view name) {
    const std::string& path = std::format("{}/resources/animation/{}", game_resource.path, name);
       
    if (auto animations = read_animations(path + "/normal"); animations[1]) {
        frames_ = animations[1]->frames();
        type2animations_[Type::NORMAL] = std::move(animations);
    }
    if (auto animations = read_animations(path + "/sketch"); animations[1]) {
        type2animations_[Type::SKETCH] = std::move(animations);
    }
    if (auto animations = read_animations(path + "/attack"); animations[1]) {
        type2animations_[Type::ATTACK] = std::move(animations);
    }
}

SDL_Texture* Animations::get_texture(int idx, int orient, Type type) {
    if (type2animations_.count(type)) {
        auto& animations = type2animations_[type];
        if (!animations[orient]->empty()) {
            return animations[orient]->get_texture(idx);
        } else if (!animations[1]->empty()) {
            return animations[1]->get_texture(idx);
        }
    }

    return type2animations_[Type::NORMAL][1]->get_texture(idx);
}

std::array<std::shared_ptr<Animation>, 4> Animations::read_animations(std::string_view path) {
    if (!std::filesystem::is_directory(std::format("{}/down", path))) {
        return {};
    }

    auto res = std::array<std::shared_ptr<Animation>, 4>();
    res[0] = std::make_shared<Animation>(std::format("{}/up", path));
    res[1] = std::make_shared<Animation>(std::format("{}/down", path));
    res[2] = std::make_shared<Animation>(std::format("{}/left", path));
    res[3] = std::make_shared<Animation>(std::format("{}/right", path));

    return res;
}

}  // namespace wheel
