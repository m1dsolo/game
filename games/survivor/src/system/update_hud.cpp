#include <survivor/system/update_hud.hpp>
#include <survivor/resource/achievement.hpp>
#include <survivor/tag/hud.hpp>

#include <core/manager/entity.hpp>
#include <core/component/hp.hpp>
#include <core/component/level.hpp>
#include <core/tag/input.hpp>
#include <core/resource/config.hpp>

#include <ecs/ecs.hpp>

#include <format>

using namespace core;

namespace survivor {

void UpdateHudSystem::operator()(wheel::ECS& ecs) {
    auto player = ecs.get_entity<InputTag>();
    const auto& hp = ecs.get_component<HPComponent>(player);
    const auto& level = ecs.get_component<LevelComponent>(player);
    const auto& config = ecs.get_resource<ConfigResource>();
    const auto& achievement = ecs.get_resource<AchievementResource>();
    auto text = std::format("level: {} exp: {}/{} hp:{}/{} kill:{}", level.level, level.exp, config.exps[level.level - 1], hp.hp, hp.max_hp, achievement.kill_num);

    auto text_entity = ecs.get_entity<HudTag>();
    EntityManager::instance().update_text(text_entity, text);
}

}  // namespace survivor
