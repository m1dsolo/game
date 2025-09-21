#pragma once

#include <core/global.hpp>

#include <wheel/singleton.hpp>

namespace core {

class GameManager : public wheel::Singleton<GameManager> {
    friend wheel::Singleton<GameManager>;

public:
    enum class State {
        MAIN_MENU,
        RUNNING,
        PAUSED,
    };

    template <typename... SystemTypes>
    void add_game_systems() {
        (add_game_system<SystemTypes>(), ...);
    }

    template <typename SystemType>
    void add_game_system() {
        ecs.add_system<SystemType>();
        ecs.pause_system<SystemType>();
        
        game_system_ids_.emplace_back(ecs.get_system_id<SystemType>());
    }

    void start();
    void stop();
    void pause();
    void resume();

    State state() const { return state_; }

private:
    GameManager() = default;
    GameManager(const GameManager&) = delete;

    std::vector<wheel::SystemID> game_system_ids_;

    State state_{State::MAIN_MENU};
};

}  // namespace core
