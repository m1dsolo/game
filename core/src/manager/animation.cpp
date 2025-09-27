#include <core/manager/animation.hpp>
#include <core/config/animation.hpp>
#include <core/manager/texture.hpp>
#include <core/manager/sprite.hpp>
#include <sdl/sdl.hpp>

#include <rfl/json.hpp>

#include <filesystem>
#include <iostream>

namespace core {

AnimationManager::AnimationManager() {
    set({{}, {SpriteManager::instance().get(sdl::SDL::WHITE)}});

    for (const auto& entry : std::filesystem::recursive_directory_iterator("assets/animation")) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            std::ifstream file(entry.path());
            if (file.is_open()) {
                std::cout << "[begin load animation...]" << entry.path() << std::endl;

                const auto [
                    name,
                    path,
                    animations,
                    default_duration,
                    default_loop,
                    default_size,
                    default_start,
                    default_step,
                    default_count
                ] = rfl::json::read<AnimationsConfig>(file).value();
                auto texture = TextureManager::instance().get(path);

                for (const auto& [
                    state,
                    anim_duration,
                    anim_loop,
                    anim_size,
                    anim_start,
                    anim_step,
                    anim_count
                ] : animations) {
                    auto duration = anim_duration.value_or(default_duration.value_or(1.f));
                    auto loop = anim_loop.value_or(default_loop.value_or(true));
                    auto size = anim_size.value_or(default_size.value_or(std::make_pair(32, 32)));
                    auto start = anim_start.value_or(default_start.value_or(std::make_pair(0, 0)));
                    auto step = anim_step.value_or(default_step.value_or(std::make_pair(0, size.second)));
                    auto count = anim_count.value_or(default_count.value_or(1));

                    std::vector<Sprite> sprites;
                    for (int i = 0; i < count; i++) {
                        sprites.emplace_back(
                            texture, SDL_FRect{
                                static_cast<float>(start.first + i * step.first),
                                static_cast<float>(start.second + i * step.second),
                                static_cast<float>(size.first),
                                static_cast<float>(size.second)
                            }
                        );
                    }
                    set(Animation{{name, state}, std::move(sprites), duration, loop});
                }
                file.close();
                std::cout << "[end load animation...]" << std::endl;
            }
        }
    }
}

void AnimationManager::set(const Animation& clip) {
    std::cout << "animation_set: " << clip.id.key << std::endl;
    clip_map_[clip.id] = clip;
}

const Animation& AnimationManager::get(const Animation::ID& id) {
    std::cout << "animation_get: " << id.name << "-" << id.state << std::endl;
    if (clip_map_.find(id) != clip_map_.end()) {
        return clip_map_.at(id);
    }
    set({id, {SpriteManager::instance().get(sdl::SDL::WHITE)}});
    return clip_map_.at(id);
}

}  // namespace core
