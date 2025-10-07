#include <core/system/level.hpp>
#include <core/global.hpp>
#include <core/component/level.hpp>
#include <core/resource/config.hpp>
#include <core/event/exp.hpp>

#include <core/global.hpp>

using namespace core;

namespace core {

void LevelSystem::operator()() {
    const auto& config = ecs.get_resource<ConfigResource>();
    for (const auto event : ecs.get_events<ExpEvent>()) {
        auto& level = ecs.get_component<LevelComponent>(event.entity);
        level.exp += event.exp;
        while (level.exp >= config.exps[level.level - 1]) {
            level.exp -= config.exps[level.level - 1];
            level.level++;
        }
    }
}

}  // namespace core
