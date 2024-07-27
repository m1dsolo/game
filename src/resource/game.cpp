#include <game/resource/game.hpp>

#include <wheel/log.hpp>

#include <game/global.hpp>

namespace wheel {

GameResource::GameResource() {
    random.set_seed(0);
}

GameResource::~GameResource() {
}

}  // namespace wheel
