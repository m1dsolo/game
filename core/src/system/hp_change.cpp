#include <core/system/hp_change.hpp>
#include <core/global.hpp>
#include <core/manager/sprite.hpp>
#include <core/manager/time.hpp>
#include <core/component/name.hpp>
#include <core/component/children.hpp>
#include <core/component/hp.hpp>
#include <core/component/loot.hpp>
#include <core/component/item.hpp>
#include <core/component/sprite.hpp>
#include <core/component/render.hpp>
#include <core/tag/input.hpp>
#include <core/tag/hp_bar.hpp>
#include <core/event/hp_change.hpp>
#include <core/entity_event/death.hpp>

using namespace core;

namespace core {

void HPChangeSystem::operator()() {
    for (auto [source, target, value] : ecs.get_events<HPChangeEvent>()) {
        if (ecs.has_component<HPComponent>(target)) {
            auto& hp = ecs.get_component<HPComponent>(target);
            if (hp.hp <= 0) {
                continue;
            }

            // update HP
            hp.hp += value;
            if (hp.hp <= 0) {
                ecs.add_entity_event(target, DeathEvent{source});
            }

            // update HP bar
            for (auto child : ecs.get_component<ChildrenComponent>(target).entities) {
                if (ecs.has_components<HPBarTag, SpriteComponent>(child)) {
                    auto bar_id = std::clamp(static_cast<int>(std::round(48.f * hp.hp / hp.max_hp)), 0, 48);
                    ecs.get_component<SpriteComponent>(child).sprite = &SpriteManager::instance().get("hp_bar" + std::to_string(bar_id));
                }
            }

            // get hit effect
            if (value < 0) {
                ecs.get_component<RenderComponent>(target).blend_mode_add_cnt++;
                TimeManager::instance().timer().add(100000, [target]() {
                    if (ecs.has_entity(target)) {
                        ecs.get_component<RenderComponent>(target).blend_mode_add_cnt--;
                    }
                    return 0;
                });
            }
        }
    }
}

}  // namespace core
