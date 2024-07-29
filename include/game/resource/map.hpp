#pragma once

#include <SDL3/SDL.h>

#include <wheel/geometry.hpp>

#include <game/tile.hpp>

namespace wheel {

struct MapResource {
    MapResource();
    ~MapResource();

    Vector2D<double> idx2pos(int i, int j) {
        return {(double)j * 48 + 24, (double)i * 48 + 24};
    }

    std::pair<int, int> pos2idx(const Vector2D<double>& pos) {
        int i = pos.y / 48, j = pos.x / 48;
        if (i < 0 || i >= tilemap.size() || j < 0 || j >= tilemap[0].size()) {
            return {-1, -1};
        }
        return {i, j};
    }

    bool plant(const std::string& name, const Vector2D<double>& position);

    TileMap tilemap;
    SDL_Texture* texture;
    std::vector<std::vector<bool>> planted;
};

}  // namespace wheel
