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

Vector2D<double> Map::idx2pos(int i, int j) const {
    return {
        j * TILE_SIZE + TILE_SIZE / 2.,
        i * TILE_SIZE + TILE_SIZE / 2.
    };
}

std::pair<int, int> Map::pos2idx(const Vector2D<double>& pos) const {
    int i = pos.y / TILE_SIZE, j = pos.x / TILE_SIZE;
    if (i < 0 || i >= tilemap_.size() || j < 0 || j >= tilemap_[0].size()) {
        return {-1, -1};
    }
    return {i, j};
}

Rect<int> Map::idx2tile_rect(int i, int j) const {
    return {
        j * TILE_SIZE,
        i * TILE_SIZE,
        (j + 1) * TILE_SIZE,
        (i + 1) * TILE_SIZE
    };
}

std::pair<int, int> Map::grid_size() const {
    return {TILE_NUM_W, TILE_NUM_H};
}

std::pair<int, int> Map::pixel_size() const {
    return {TILE_NUM_W * TILE_SIZE, TILE_NUM_H * TILE_SIZE};
}

bool Map::is_in_bound(const Vector2D<double>& pos) {
    return pos.x >= 0 &&
        pos.x <= TILE_NUM_W * TILE_SIZE &&
        pos.y >= 0 &&
        pos.y <= TILE_NUM_H * TILE_SIZE;
}

bool Map::plant(const std::string& name, Entity entity, const Vector2D<double>& position) {
    auto [i, j] = pos2idx(position);
    if (i < 0 || i >= TILE_NUM_H || j < 0 || j >= TILE_NUM_W) {
        return false;
    }

    auto pos = idx2pos(i, j);
    if (name.find("floor") != std::string::npos) {
        // if (planted_floor_[i][j]) {
        //     return false;
        // }
        // planted_floor_[i][j] = true;
        // sdl.set_target(texture_);
        // auto texture = TextureManager::instance().get_texture(name);
        // SDL_FRect dst = {(float)pos.x - (float)TILE_SIZE / 2, (float)pos.y - (float)TILE_SIZE / 2, TILE_SIZE, TILE_SIZE};
        // sdl.render(texture, nullptr, &dst);
        // sdl.set_target(nullptr);
    } else {
        if (planted_structure_[i][j]) {
            return false;
        }
        planted_structure_[i][j] = true;
        if (name.find("tower") != std::string::npos) {
            EntityManager::instance().create_tower(name, entity, pos);
        } else {
            EntityManager::instance().create_structure(name, entity, pos);
        }
    }

    return true;
}

bool Map::is_planted(int i, int j) const {
    if (i < 0 || i >= TILE_NUM_H || j < 0 || j >= TILE_NUM_W) {
        return false;
    }
    return planted_structure_[i][j];
}

bool Map::is_collision(const Rect<double>& rect) const {
    return is_collision(Rect<int>(rect));
}

bool Map::is_collision(const Rect<int>& rect) const {
    for (int i = rect.y0; i < rect.y1 + TILE_SIZE - 1; i += TILE_SIZE) {
        i = std::min(i, rect.y1 - 1);
        for (int j = rect.x0; j < rect.x1 + TILE_SIZE - 1; j += TILE_SIZE) {
            j = std::min(j, rect.x1 - 1);
            auto idx = pos2idx({(double)j, (double)i});
            auto tile_rect = idx2tile_rect(idx.first, idx.second);
            if (is_planted(idx.first, idx.second) && rect.is_overlapping(tile_rect)) {
                return true;
            }
        }
    }
    return false;
}

void Map::generate_map_texture() {
	const siv::PerlinNoise::seed_type seed = 123456u;
	const siv::PerlinNoise perlin{seed};
	
    auto& texture_manager = TextureManager::instance();
    texture_ = sdl.create_texture((TILE_NUM_W + 2 * PADDING_TILE_NUM) * TILE_SIZE, (TILE_NUM_H + 2 * PADDING_TILE_NUM) * TILE_SIZE, sdl.BLACK, SDL_TEXTUREACCESS_TARGET);
    sdl.set_target(texture_);
    tilemap_ = std::vector<std::vector<Tile>>(TILE_NUM_H, std::vector<Tile>(TILE_NUM_W));
    for (int i = 0; i < TILE_NUM_H; i++) {
        for (int j = 0; j < TILE_NUM_W; j++) {
            float x = (j + PADDING_TILE_NUM) * TILE_SIZE, y = (i + PADDING_TILE_NUM) * TILE_SIZE;
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

    game_rect_ = {
        0,
        0,
        TILE_NUM_W * TILE_SIZE,
        TILE_NUM_H * TILE_SIZE
    };
    real_rect_ = {
        PADDING_TILE_NUM * TILE_SIZE,
        PADDING_TILE_NUM * TILE_SIZE,
        (TILE_NUM_W + PADDING_TILE_NUM) * TILE_SIZE,
        (TILE_NUM_H + PADDING_TILE_NUM) * TILE_SIZE
    };
}

}  // namespace wheel
