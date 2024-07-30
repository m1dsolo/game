#pragma once

#include <vector>

namespace wheel {

struct Tile {
    enum class Type {
        NONE,
        GRASS,
        SAND,
        WATER
    };

    Tile(Type type = Type::NONE) : type(type) {}

    Type type;
};

using TileMap = std::vector<std::vector<Tile>>;

}  // namespace wheel
