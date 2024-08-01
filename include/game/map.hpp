#pragma once

#include <SDL3/SDL.h>

#include <wheel/geometry.hpp>
#include <wheel/singleton.hpp>

#include <game/tile.hpp>

namespace wheel {

class Map : public Singleton<Map> {
    friend class Singleton<Map>;

public:
    Vector2D<double> idx2pos(int i, int j);
    std::pair<int, int> pos2idx(const Vector2D<double>& pos);

    bool plant(const std::string& name, const Vector2D<double>& position);

    SDL_Texture* texture() { return texture_; }
    const SDL_FRect& dst() const { return map_dst_; }

    static const int TILE_SIZE = 48;

private:
    Map();
    ~Map();

    void generate_map_texture();

    TileMap tilemap_;
    SDL_Texture* texture_;
    std::vector<std::vector<bool>> planted_;
    SDL_FRect map_dst_;
};

}  // namespace wheel

