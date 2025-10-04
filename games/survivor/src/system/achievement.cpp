#include <survivor/system/achievement.hpp>
#include <survivor/manager/achievement.hpp>
#include <core/global.hpp>
#include <core/tag/input.hpp>
#include <core/entity_event/death.hpp>

using namespace core;

namespace survivor {

void AchievementSystem::operator()() {
    auto player_entity = ecs.get_entity<InputTag>();
    for (auto [event] : ecs.get_components<DeathEvent>()) {
        if (event.source == player_entity) {
            AchievementManager::instance().add_kill();
        }
    }
}

}  // namespace survivor
