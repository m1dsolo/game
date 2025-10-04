#include <core/system/level.hpp>
#include <core/global.hpp>
#include <core/component/level.hpp>
#include <core/event/exp.hpp>

#include <core/global.hpp>

using namespace core;

namespace core {

void LevelSystem::operator()() {
    for (const auto [entity, level, event] : ecs.get_entity_and_components<LevelComponent, ExpEvent>()) {
        level.exp += event.exp;
        while (level.exp >= config.exps[level.level - 1]) {
            level.exp -= config.exps[level.level - 1];
            level.level++;
        }
    }
}

}  // namespace core
