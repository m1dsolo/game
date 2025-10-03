#include <survivor/system/achievement.hpp>
#include <survivor/manager/achievement.hpp>
#include <core/global.hpp>
#include <core/tag/input.hpp>
#include <core/event/death.hpp>

using namespace core;

namespace survivor {

void AchievementSystem::operator()() {
    auto player_entity = ecs.get_entity<InputTag>();
    for (auto [source, target] : ecs.get_events<DeathEvent>()) {
        if (source == player_entity) {
            AchievementManager::instance().add_kill();
        }
    }
}

}  // namespace survivor
