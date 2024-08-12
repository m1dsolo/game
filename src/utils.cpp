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
        auto texture = sdl.load_img(s.c_str());
        res.emplace_back(texture);
    }

    return res;
}

Vector2D<float> GameUtils::gen_spawn_boundary_position() {
    Vector2D<float> position;
    int direction = random.uniform(0, 3);
    auto [x0, y0, x1, y1] = Map::instance().game_rect();
    switch (direction) {
        // up
        case 0: {
            position = {(float)random.uniform(x0, x1 - 1), (float)y0};
            break;
        }
        // down
        case 1: {
            position = {(float)random.uniform(x0, x1 - 1), (float)y1 - 1};
            break;
        }
        // left
        case 2: {
            position = {(float)x0, (float)random.uniform(y0, y1 - 1)};
            break;
        }
        // right
        case 3: {
            position = {(float)x1 - 1, (float)random.uniform(y0, y1 - 1)};
            break;
        }
    }

    return position;
}

Vector2D<float> GameUtils::gen_spawn_internal_position(int margin) {
    auto size = Map::instance().pixel_size();
    float w = random.uniform(margin, (int)size.first - margin);
    float h = random.uniform(margin, (int)size.second - margin);
    return {w, h};
}

int GameUtils::direction2orientation(const Vector2D<float>& direction) {
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
