#include <core/manager/sprite.hpp>
#include <core/config/sprite.hpp>
#include <core/manager/texture.hpp>
#include <sdl/sdl.hpp>

#include <rfl/json.hpp>

#include <filesystem>
#include <iostream>

namespace core {

SpriteManager::SpriteManager() {
    set("", {sdl::SDL::create_texture(48, 48, sdl::SDL::PURPLE), {0.f, 0.f, 48.f, 48.f}});

    for (const auto& entry : std::filesystem::recursive_directory_iterator("assets/sprite")) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            std::ifstream file(entry.path());
            if (file.is_open()) {
                std::cout << "[begin load sprite...]" << entry.path() << std::endl;
                const auto [path, size, sprites] = rfl::json::read<SpritesConfig>(file).value();
                auto texture = TextureManager::instance().get(path);
                for (const auto& [name, pos] : sprites) {
                    set(
                        name, {
                            texture, {
                                static_cast<float>(pos.first),
                                static_cast<float>(pos.second),
                                static_cast<float>(size.first),
                                static_cast<float>(size.second)
                            }
                        }
                    );
                }
                file.close();
                std::cout << "[end load sprite...]" << std::endl;
            }
        }
    }
}

void SpriteManager::del(const std::string& name) {
    if (auto iter = name2sprites_.find(name); iter != name2sprites_.end()) {
        sdl::SDL::destroy(iter->second.texture);
        name2sprites_.erase(iter);
    }
}

}  // namespace core
