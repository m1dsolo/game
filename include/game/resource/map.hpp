#pragma once

#include <SDL3/SDL.h>

#include <wheel/geometry.hpp>

#include <game/tile.hpp>

namespace wheel {

struct MapResource {
    MapResource();
    ~MapResource();

    Vector2D<double> idx2pos(int i, int j) {
        return {(double)j * TILE_SIZE + TILE_SIZE / 2., (double)i * TILE_SIZE + TILE_SIZE / 2.};
    }

    std::pair<int, int> pos2idx(const Vector2D<double>& pos) {
        int i = pos.y / TILE_SIZE, j = pos.x / TILE_SIZE;
        if (i < 0 || i >= tilemap.size() || j < 0 || j >= tilemap[0].size()) {
            return {-1, -1};
        }
        return {i, j};
    }

    bool plant(const std::string& name, const Vector2D<double>& position);

    TileMap tilemap;
    SDL_Texture* texture;
    std::vector<std::vector<bool>> planted;

    static const int TILE_SIZE = 48;
};

}  // namespace wheel
