#include <core/manager/game.hpp>
#include <core/global.hpp>
#include <core/manager/ui.hpp>
#include <core/layer/main_menu.hpp>

namespace core {

void GameManager::start() {
    if (state_ != State::RUNNING) {
        state_ = State::RUNNING;
        for (const auto& system_id : game_system_ids_) {
            ecs.resume_system(system_id);
        }
    }
}

void GameManager::stop() {
    if (state_ != State::MAIN_MENU) {
        state_ = State::MAIN_MENU;
        for (const auto& system_id : game_system_ids_) {
            ecs.pause_system(system_id);
        }
        UIManager::instance().push_back<MainMenuLayer>();
    }
}

void GameManager::pause() {
    if (state_ == State::RUNNING) {
        state_ = State::PAUSED;
        for (const auto& system_id : game_system_ids_) {
            ecs.pause_system(system_id);
        }
        UIManager::instance().push_back<MainMenuLayer>();
    }
}

void GameManager::resume() {
    if (state_ != State::RUNNING) {
        state_ = State::RUNNING;
        for (const auto& system_id : game_system_ids_) {
            ecs.resume_system(system_id);
        }
        UIManager::instance().pop_back();
    }
}

}  // namespace core
