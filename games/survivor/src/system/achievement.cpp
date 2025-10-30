#include <survivor/system/achievement.hpp>
#include <survivor/resource/achievement.hpp>

#include <core/tag/input.hpp>
#include <core/entity_event/death.hpp>

#include <ecs/ecs.hpp>

using namespace core;

namespace survivor {

void AchievementSystem::operator()(wheel::ECS& ecs) {
    auto player_entity = ecs.get_entity<InputTag>();
    auto& achievement = ecs.get_resource<AchievementResource>();
    for (auto [event] : ecs.get_components<DeathEvent>()) {
        if (event.source == player_entity) {
            achievement.kill_num++;
        }
    }
}

}  // namespace survivor
