#include <core/system/level.hpp>
#include <core/component/level.hpp>
#include <core/component/inventory.hpp>
#include <core/resource/config.hpp>
#include <core/event/exp.hpp>

#include <ecs/ecs.hpp>

namespace core {

void LevelSystem::operator()(wheel::ECS& ecs) {
    const auto& config = ecs.get_resource<ConfigResource>();
    for (const auto [entity, exp] : ecs.get_events<ExpEvent>()) {
        auto& level = ecs.get_component<LevelComponent>(entity);
        level.exp += exp;
        while (level.exp >= config.exps[level.level - 1]) {
            level.exp -= config.exps[level.level - 1];
            level.level++;

            if (ecs.has_component<InventoryComponent>(entity)) {
                auto& inventory = ecs.get_component<InventoryComponent>(entity);
                inventory.max_weapon_weight++;
                inventory.max_equipment_weight++;
            }
        }
    }
}

}  // namespace core
