#pragma once

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <SDL3_ttf/SDL_ttf.h>


namespace wheel {

class GameManager final {
public:
    GameManager();
    ~GameManager();

    void run();
};

}  // namespace game
