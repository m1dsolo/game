#include "core/game.hpp"
#include <core/layer/game.hpp>
#include <core/global.hpp>
#include <core/manager/game.hpp>
#include <core/manager/ui.hpp>
#include <core/system/move.hpp>

#include <sdl/sdl.hpp>

namespace core {

void GameLayer::on_attach() {
    GameManager::instance().start();
}

void GameLayer::on_detach() {
    GameManager::instance().stop();
}

bool GameLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_W: MoveSystem::is_move_up = true; return true;
                case SDLK_S: MoveSystem::is_move_down = true; return true;
                case SDLK_A: MoveSystem::is_move_left = true; return true;
                case SDLK_D: MoveSystem::is_move_right = true; return true;
                case SDLK_ESCAPE: {
                    GameManager::instance().pause();
                    return true;
                }
            }
            break;
        }
        case SDL_EVENT_KEY_UP: {
            switch (event.key.key) {
                case SDLK_W: MoveSystem::is_move_up = false; return true;
                case SDLK_S: MoveSystem::is_move_down = false; return true;
                case SDLK_A: MoveSystem::is_move_left = false; return true;
                case SDLK_D: MoveSystem::is_move_right = false; return true;
            }
            break;
        }
        // case SDL_Event_GAMEPAD_ADDED
        case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
            const auto& gaxis = event.gaxis;
            switch (gaxis.axis) {
                case SDL_GAMEPAD_AXIS_LEFTX: {
                    float value = gaxis.value / 32767.0f;
                    MoveSystem::is_move_left = (value < -0.25);
                    MoveSystem::is_move_right = (value > 0.25);
                    return true;
                }
                case SDL_GAMEPAD_AXIS_LEFTY: {
                    float value = gaxis.value / 32767.0f;
                    MoveSystem::is_move_up = (value < -0.25);
                    MoveSystem::is_move_down = (value > 0.25);
                    return true;
                }
            }
            break;
        }
    }

    return false;
}

}  // namespace core
