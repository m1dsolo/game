#pragma once

#include <game/component/item.hpp>

namespace wheel {

struct PlayerResource {
    PlayerResource();
    ~PlayerResource();

    int killed = 0;
};

}  // namespace wheel
