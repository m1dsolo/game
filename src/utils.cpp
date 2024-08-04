#include <game/utils.hpp>

#include <filesystem>

#include <game/global.hpp>
#include <game/map.hpp>

namespace wheel {

int GameUtils::get_file_count(std::string_view path) {
    return std::distance(std::filesystem::directory_iterator(path), std::filesystem::directory_iterator{});
}

std::vector<SDL_Texture*> GameUtils::read_textures(std::string_view path) {
    std::vector<SDL_Texture*> res;
    int n = get_file_count(path);
    for (int i = 0; i < n; i++) {
        std::string s = std::format("{}/{}.png", path, i);
        SDL_Texture* texture = IMG_LoadTexture(renderer, s.c_str());
        res.emplace_back(texture);
    }

    return res;
}

Vector2D<double> GameUtils::gen_spawn_boundary_position() {
    Vector2D<double> position;
    int direction = random.uniform(0, 3);
    auto& [pos, size] = Map::instance().rect();
    int w = size.x, h = size.y;
    int x0 = pos.x;
    int y0 = pos.y;
    int x1 = pos.x + w;
    int y1 = pos.y + h;
    switch (direction) {
        // up
        case 0: {
            position = {(double)random.uniform(x0, x1 - 1), (double)y0};
            break;
        }
        // down
        case 1: {
            position = {(double)random.uniform(x0, x1 - 1), (double)y1 - 1};
            break;
        }
        // left
        case 2: {
            position = {(double)x0, (double)random.uniform(y0, y1 - 1)};
            break;
        }
        // right
        case 3: {
            position = {(double)x1 - 1, (double)random.uniform(y0, y1 - 1)};
            break;
        }
    }

    return position;
}

Vector2D<double> GameUtils::gen_spawn_internal_position(int margin) {
    auto& [pos, size] = Map::instance().rect();
    double w = random.uniform(margin, (int)size.x - margin);
    double h = random.uniform(margin, (int)size.y - margin);
    return {w, h};
}

int GameUtils::direction2orientation(const Vector2D<double>& direction) {
    if (std::abs(direction.y) >= std::abs(direction.x)) {
        if (direction.y < 0) {
            return 0;
        } else {
            return 1;
        }
    } else {
        if (direction.x < 0) {
            return 2;
        } else {
            return 3;
        }
    }
    return 1;
}

}  // namespace wheel
