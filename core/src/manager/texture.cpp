#include <core/manager/texture.hpp>
#include <core/global.hpp>
#include <filesystem>
#include <sdl/sdl.hpp>

namespace fs = std::filesystem;

namespace core {

TextureManager::TextureManager() {
    // default
    set("", sdl::SDL::create_texture(48, 48, sdl::SDL::Color::White));

    // TODO: ugly code
    for (const auto& entry : fs::recursive_directory_iterator("assets/sprite_sheet")) {
        if (entry.is_regular_file() && entry.path().extension() == ".png") {
            auto path = entry.path();
            auto key = path;
            if (fs::is_symlink(path)) {
                key = fs::relative(path, fs::read_symlink("assets/sprite_sheet"));
            } else {
                key = fs::relative(path, "assets/sprite_sheet");
            }
            auto texture = sdl::SDL::load_image(path);
            sdl::SDL::set_texture_scalemode(texture, SDL_SCALEMODE_NEAREST);
            set("sprite_sheet" / key, texture);
        }
    }
}

}  // namespace core
