#pragma once

#include <game/tile.hpp>

#include <SDL3/SDL.h>

namespace wheel {

struct MapResource {
    MapResource();
    ~MapResource();

    TileMap tilemap;
    SDL_Texture* texture;
};

}  // namespace wheel
