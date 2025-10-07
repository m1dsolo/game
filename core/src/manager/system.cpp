#include <core/manager/system.hpp>
#include <core/global.hpp>

namespace core {

void SystemManager::pause_game_systems() {
    for (const auto& system_id : game_system_ids_) {
        ecs.pause_system(system_id);
    }
}

void SystemManager::resume_game_systems() {
    for (const auto& system_id : game_system_ids_) {
        ecs.resume_system(system_id);
    }
}

}  // namespace core
