#pragma once

#include <core/global.hpp>

#include <wheel/singleton.hpp>

namespace core {

class SystemManager : public wheel::Singleton<SystemManager> {
    friend wheel::Singleton<SystemManager>;

public:
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

    void pause_game_systems();
    void resume_game_systems();

private:
    SystemManager() = default;
    SystemManager(const SystemManager&) = delete;

    std::vector<wheel::SystemID> game_system_ids_;
};

}  // namespace core
