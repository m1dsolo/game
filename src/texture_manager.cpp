#include <game/texture_manager.hpp>

#include <filesystem>

#include <wheel/json.hpp>

#include <game/global.hpp>

namespace wheel {

TextureManager::TextureManager() {
    // load textures
    const std::string& texture_path = game_resource.path / "resources" / "texture";
    for (const auto& entry : std::filesystem::directory_iterator(texture_path)) {
        Log::debug("TextureManager: load texture {}", entry.path().stem().string());
        const std::string& name = entry.path().stem().string();
        texture_map_[name] = sdl.load_img(entry.path());
    }

    // crop sheets to textures
    const std::string& sheet_path = game_resource.path / "resources" / "sheet";
    JsonListType obj = Json::parse_file(game_resource.path / "sheet.json");
    for (JsonDictType sheet : obj) {
        std::string name = sheet["name"];
        auto texture = sdl.load_img(std::format("{}/{}.png", sheet_path, name));
        JsonListType tiles = sheet["tiles"];
        int w = sheet["tile_size"][0];
        int h = sheet["tile_size"][1];
        for (JsonDictType tile : tiles) {
            std::string tile_name = tile["name"];
            int tile_x = tile["x"];
            int tile_y = tile["y"];
            int tile_w = w;
            int tile_h = h;
            if (tile.find("w") != tile.end()) {
                tile_w = tile["w"];
                tile_h = tile["h"];
            }
            SDL_Texture* tile_texture = sdl.create_texture(tile_w, tile_h, sdl.BLACK, SDL_TEXTUREACCESS_TARGET);
            sdl.set_target(tile_texture);
            SDL_FRect src = {(float)tile_x, (float)tile_y, (float)tile_w, (float)tile_h};
            sdl.render(texture, &src, nullptr);
            sdl.set_target(nullptr);
            texture_map_[tile_name] = tile_texture;
        }
    }

    // cursor texture
    texture_map_["circle_cursor"] = sdl.create_filled_circle_texture(5, SDL::RED);
}

TextureManager::~TextureManager() {
    for (auto& texture : texture_map_) {
        sdl.destroy(texture.second);
    }
}

SDL_Texture* TextureManager::get_texture(const std::string& name, int w, int h) {
    if (texture_map_.count(name)) {
        return texture_map_.at(name);
    } else {
        auto texture = sdl.create_texture(w, h, sdl.BLACK, SDL_TEXTUREACCESS_TARGET);
        SDL_FRect dst = {0, 0, (float)w, (float)h};
        sdl.set_target(texture);
        sdl.draw_text(name.substr(0, 8), &dst, sdl.BLACK, true, w, h);
        sdl.set_target(nullptr);
        return texture_map_[name] = texture;
    }
}

}  // namespace wheel
