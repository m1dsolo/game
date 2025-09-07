#include <core/manager/texture.hpp>
#include <core/global.hpp>
#include <filesystem>
#include <sdl/sdl.hpp>

namespace fs = std::filesystem;

namespace core {

TextureManager::TextureManager() {
    // default
    set("", sdl::SDL::create_texture(48, 48, sdl::SDL::WHITE));

    // TODO: ugly code
    for (const auto& entry : fs::recursive_directory_iterator("assets/sprite_sheet")) {
        if (entry.is_regular_file() && entry.path().extension() == ".png") {
            auto path = fs::relative(entry.path(), fs::read_symlink("assets/sprite_sheet"));
            set("sprite_sheet" / path, sdl::SDL::load_image(entry.path()));
        }
    }
}

}  // namespace core
