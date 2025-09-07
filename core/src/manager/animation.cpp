#include <core/manager/animation.hpp>
#include <core/config/animation.hpp>
#include <core/manager/texture.hpp>
#include <sdl/sdl.hpp>

#include <rfl/json.hpp>

#include <filesystem>

namespace core {

AnimationManager::AnimationManager() {
    set("", {"", {Sprite{ sdl::SDL::create_texture(48, 48, sdl::SDL::WHITE), {0.f, 0.f, 48.f, 48.f} }}});

    for (const auto& entry : std::filesystem::recursive_directory_iterator("assets/animation")) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            std::ifstream file(entry.path());
            if (file.is_open()) {
                std::cout << "[begin load animation...]" << entry.path() << std::endl;
                const auto [id, path, duration, loop, size, range, animations] = rfl::json::read<AnimationsConfig>(file).value();
                const auto& [_, step, count] = range;
                auto texture = TextureManager::instance().get(path);

                for (const auto& [name, clips] : animations) {
                    for (const auto& [direction, range] : clips) {
                        std::vector<Sprite> sprites;
                        auto start = range.start;
                        for (int i = 0; i < count; i++) {
                            sprites.emplace_back(
                                texture, SDL_FRect{
                                    static_cast<float>(start->first + i * step->first),
                                    static_cast<float>(start->second + i * step->second),
                                    static_cast<float>(size.first),
                                    static_cast<float>(size.second)
                                }
                            );
                            auto& sprite = sprites.back();
                        }
                        // TODO: ugly code
                        set(id + "-" + name + "-" + direction, Animation{id + "-" + name, std::move(sprites), duration, loop});
                    }
                }
                file.close();
                std::cout << "[end load animation...]" << std::endl;
            }
        }
    }
}

}  // namespace core
