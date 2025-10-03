#include <core/system/level.hpp>
#include <core/global.hpp>
#include <core/component/level.hpp>
#include <core/event/exp.hpp>

#include <core/global.hpp>

using namespace core;

namespace core {

void LevelSystem::operator()() {
    for (const auto [entity, exp] : ecs.get_events<ExpEvent>()) {
        if (ecs.has_component<LevelComponent>(entity)) {
            auto& level = ecs.get_component<LevelComponent>(entity);
            level.exp += exp;
            while (level.exp >= config.exps[level.level - 1]) {
                level.exp -= config.exps[level.level - 1];
                level.level++;
            }
        }
    }
}

}  // namespace core
