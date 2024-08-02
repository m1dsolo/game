#include <game/map.hpp>

#include <PerlinNoise.hpp>

#include <wheel/csv.hpp>
#include <wheel/ecs.hpp>
#include <wheel/log.hpp>

#include <game/global.hpp>
#include <game/texture_manager.hpp>
#include <game/entity_manager.hpp>

#include <game/component/self.hpp>

namespace wheel {

Map::Map() {
    generate_map_texture();
    planted_structure_ = std::vector<std::vector<bool>>(TILE_NUM_H, std::vector<bool>(TILE_NUM_W));
    planted_floor_ = std::vector<std::vector<bool>>(TILE_NUM_H, std::vector<bool>(TILE_NUM_W));
}

Map::~Map() {

}

Vector2D<double> Map::idx2pos(int i, int j) {
    return {(double)j * TILE_SIZE + TILE_SIZE / 2., (double)i * TILE_SIZE + TILE_SIZE / 2.};
}

std::pair<int, int> Map::pos2idx(const Vector2D<double>& pos) {
    int i = pos.y / TILE_SIZE, j = pos.x / TILE_SIZE;
    if (i < 0 || i >= tilemap_.size() || j < 0 || j >= tilemap_[0].size()) {
        return {-1, -1};
    }
    return {i, j};
}

bool Map::plant(const std::string& name, Entity entity, const Vector2D<double>& position) {
    auto [i, j] = pos2idx(position - Vector2D<double>(map_dst_.x, map_dst_.y));
    if (i < 0 || i >= TILE_NUM_H || j < 0 || j >= TILE_NUM_W) {
        return false;
    }

    auto pos = idx2pos(i, j);
    if (name.find("floor") != std::string::npos) {
        if (planted_floor_[i][j]) {
            return false;
        }
        planted_floor_[i][j] = true;
        sdl.set_target(texture_);
        auto texture = TextureManager::instance().get_texture(name);
        SDL_FRect dst = {(float)pos.x - (float)TILE_SIZE / 2, (float)pos.y - (float)TILE_SIZE / 2, TILE_SIZE, TILE_SIZE};
        sdl.render(texture, nullptr, &dst);
        sdl.set_target(nullptr);
    } else {
        if (planted_structure_[i][j]) {
            return false;
        }
        planted_structure_[i][j] = true;
        if (name.find("tower") != std::string::npos) {
            EntityManager::instance().create_tower(name, entity, pos + Vector2D<double>(map_dst_.x, map_dst_.y));
        } else {
            EntityManager::instance().create_structure(name, entity, pos + Vector2D<double>(map_dst_.x, map_dst_.y));
        }
    }

    return true;
}

void Map::generate_map_texture() {
	const siv::PerlinNoise::seed_type seed = 123456u;
	const siv::PerlinNoise perlin{seed};
	
    auto& texture_manager = TextureManager::instance();
    texture_ = sdl.create_texture(TILE_NUM_W * TILE_SIZE, TILE_NUM_H * TILE_SIZE, sdl.BLACK, SDL_TEXTUREACCESS_TARGET);
    sdl.set_target(texture_);
    tilemap_ = std::vector<std::vector<Tile>>(TILE_NUM_H, std::vector<Tile>(TILE_NUM_W));
    for (int i = 0; i < TILE_NUM_H; i++) {
        for (int j = 0; j < TILE_NUM_W; j++) {
            float x = j * TILE_SIZE, y = i * TILE_SIZE;
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
            SDL_FRect dst = {x, y, TILE_SIZE, TILE_SIZE};
            tilemap_[i][j] = {type};
            sdl.render(tile_texture, nullptr, &dst);
		}
	}
    sdl.set_target(nullptr);

    map_dst_ = {
        (config_resource.w - (float)TILE_NUM_W * TILE_SIZE) / 2,
        (config_resource.h - (float)TILE_NUM_H * TILE_SIZE) / 2,
        (float)TILE_NUM_W * TILE_SIZE,
        (float)TILE_NUM_H * TILE_SIZE
    };
}

}  // namespace wheel
