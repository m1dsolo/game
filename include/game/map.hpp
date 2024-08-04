#pragma once

#include <SDL3/SDL.h>

#include <wheel/ecs.hpp>
#include <wheel/geometry.hpp>
#include <wheel/singleton.hpp>

#include <game/tile.hpp>

namespace wheel {

class Map : public Singleton<Map> {
    friend class Singleton<Map>;

public:
    Vector2D<double> idx2pos(int i, int j);
    std::pair<int, int> pos2idx(const Vector2D<double>& pos);

    // [w, h]
    std::pair<int, int> grid_size() const;
    const Rect<double>& rect() const { return rect_; }

    static bool is_in_bound(const Vector2D<double>& pos);

    // position is the world coordinate system
    bool plant(const std::string& name, Entity entity, const Vector2D<double>& position);

    SDL_Texture* texture() { return texture_; }

    static const int TILE_SIZE = 48;
    static inline const int TILE_NUM_W = 30;
    static inline const int TILE_NUM_H = 20;
    static inline const int PADDING_TILE_NUM = 20;

private:
    Map();
    ~Map();

    void generate_map_texture();

    TileMap tilemap_;
    SDL_Texture* texture_;
    std::vector<std::vector<bool>> planted_structure_;
    std::vector<std::vector<bool>> planted_floor_;
    Rect<double> rect_;
};

}  // namespace wheel

