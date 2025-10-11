#include <survivor/system/achievement.hpp>
#include <survivor/manager/achievement.hpp>

#include <core/tag/input.hpp>
#include <core/entity_event/death.hpp>

#include <ecs/ecs.hpp>

using namespace core;

namespace survivor {

void AchievementSystem::operator()(wheel::ECS& ecs) {
    auto player_entity = ecs.get_entity<InputTag>();
    for (auto [event] : ecs.get_components<DeathEvent>()) {
        if (event.source == player_entity) {
            AchievementManager::instance().add_kill();
        }
    }
}

}  // namespace survivor
