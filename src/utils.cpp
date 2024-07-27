#include <game/utils.hpp>

#include <filesystem>

#include <game/global.hpp>

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
    switch (direction) {
        // up
        case 0: {
            position = {(double)random.uniform(0, config_resource.w - 1), 0};
            break;
        }
        // down
        case 1: {
            position = {(double)random.uniform(0, config_resource.w - 1), (double)config_resource.h};
            break;
        }
        // left
        case 2: {
            position = {0, (double)random.uniform(0, config_resource.h - 1)};
            break;
        }
        // right
        case 3: {
            position = {(double)config_resource.w, (double)random.uniform(0, config_resource.h - 1)};
            break;
        }
    }

    return position;
}

Vector2D<double> GameUtils::gen_spawn_internal_position(int margin) {
    double w = random.uniform(margin, config_resource.w - margin);
    double h = random.uniform(margin, config_resource.h - margin);
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
