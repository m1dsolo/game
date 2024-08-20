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
    Vector2D<float> idx2pos(int i, int j) const;
    std::pair<int, int> pos2idx(const Vector2D<float>& pos) const;
    Rect<int> idx2tile_rect(int i, int j) const;

    // [w, h]
    std::pair<int, int> grid_size() const;
    std::pair<int, int> pixel_size() const;
    const Rect<int>& game_rect() const { return game_rect_; }
    const Rect<int>& real_rect() const { return real_rect_; }

    static bool is_in_bound(const Vector2D<float>& pos);

    // position is the world coordinate system
    bool plant(const std::string& name, Entity entity, const Vector2D<float>& position);

    bool is_planted(int i, int j) const;
    bool is_collision(const Rect<int>& rect) const;

    SDL_Texture* texture() { return texture_; }

    const std::vector<std::vector<bool>>& planted_structure() const { return planted_structure_; }

    static const int TILE_SIZE = 48;
    static inline const int TILE_NUM_W = 35;
    static inline const int TILE_NUM_H = 20;
    static inline const int PADDING_TILE_NUM = 20;

private:
    Map();
    ~Map();

    void generate_map_texture();

    TileMap tilemap_;
    SDL_Texture* texture_;
    std::vector<std::vector<bool>> planted_structure_;
    std::vector<std::vector<bool>> planted_tower_;
    std::vector<std::vector<bool>> planted_floor_;
    Rect<int> game_rect_;
    Rect<int> real_rect_;
};

}  // namespace wheel

