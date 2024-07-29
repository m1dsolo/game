#include <game/resource/map.hpp>

#include <PerlinNoise.hpp>

#include <wheel/csv.hpp>
#include <wheel/ecs.hpp>
#include <wheel/log.hpp>

#include <game/global.hpp>
#include <game/texture_manager.hpp>
#include <game/entity_manager.hpp>

#include <game/component/self.hpp>

namespace wheel {

MapResource::MapResource() {
	const siv::PerlinNoise::seed_type seed = 123456u;
	const siv::PerlinNoise perlin{seed};
	
    auto& texture_manager = TextureManager::instance();
    int map_w = config_resource.map_w, map_h = config_resource.map_h;
    texture = sdl.create_texture(map_w * 48, map_h * 48, sdl.BLACK, SDL_TEXTUREACCESS_TARGET);
    sdl.set_target(texture);
    tilemap = std::vector<std::vector<Tile>>(map_h, std::vector<Tile>(map_w));
    planted = std::vector<std::vector<bool>>(map_h, std::vector<bool>(map_w));
    for (int i = 0; i < map_h; i++) {
        for (int j = 0; j < map_w; j++) {
            float x = j * 48, y = i * 48;
			const double noise = perlin.octave2D_01((x * 0.01), (y * 0.01), 8);
            std::string tile_name = "";
            Tile::Type type = Tile::Type::NONE;
            if (noise <= 0.2) {
                tile_name = "grass0";
                type = Tile::Type::GRASS;
            } else if (noise <= 0.4) {
                tile_name = "grass1";
                type = Tile::Type::GRASS;
            } else if (noise <= 0.6) {
                tile_name = "grass2";
                type = Tile::Type::GRASS;
            } else if (noise <= 0.7) {
                tile_name = "sand0";
                type = Tile::Type::SAND;
            } else if (noise <= 0.75) {
                tile_name = "sand1";
                type = Tile::Type::SAND;
            } else if (noise <= 0.8) {
                tile_name = "sand_water0";
                type = Tile::Type::WATER;
            } else if (noise <= 1.) {
                tile_name = "water0";
                type = Tile::Type::WATER;
            }
            auto tile_texture = texture_manager.get_texture(tile_name);
            SDL_FRect dst = {x, y, 48, 48};
            tilemap[i][j] = {type};
            sdl.render(tile_texture, nullptr, &dst);
		}
	}

    sdl.set_target(nullptr);
}

MapResource::~MapResource() {

}

bool MapResource::plant(const std::string& name, const Vector2D<double>& position) {
    auto [i, j] = pos2idx(position);
    if (i < 0 || i >= planted.size() || j < 0 || j >= planted[0].size() || planted[i][j]) {
        return false;
    }

    auto pos = idx2pos(i, j);
    Entity entity = ecs.get_entities<SelfComponent>()[0];
    EntityManager::instance().create_tower(name, entity, pos);
    return true;
}

}  // namespace wheel
