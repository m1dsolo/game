#include <survivor/system/level.hpp>
#include <survivor/global.hpp>
#include <survivor/component/level.hpp>
#include <survivor/event/exp.hpp>

#include <core/global.hpp>

using namespace core;

namespace survivor {

void LevelSystem::operator()() {
    for (const auto [entity, exp] : ecs.get_events<ExpEvent>()) {
        if (ecs.has_component<LevelComponent>(entity)) {
            auto& level = ecs.get_component<LevelComponent>(entity);
            level.exp += exp;
            while (level.exp >= game_config.exps[level.level - 1]) {
                level.exp -= game_config.exps[level.level - 1];
                level.level++;
            }
        }
    }
}

}  // namespace survivor
