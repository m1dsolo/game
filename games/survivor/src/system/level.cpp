#include <survivor/system/level.hpp>
#include <core/global.hpp>
#include <survivor/component/level.hpp>
#include <survivor/event/exp.hpp>

using namespace core;

namespace survivor {

void LevelSystem::operator()() {
    for (const auto [entity, exp] : ecs.get_events<ExpEvent>()) {
        if (ecs.has_component<LevelComponent>(entity)) {
            auto& level = ecs.get_component<LevelComponent>(entity);
            level.exp += exp;
        }
    }
}

}  // namespace survivor
