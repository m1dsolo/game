#include <survivor/system/event.hpp>
#include <core/global.hpp>
#include <core/manager/sprite.hpp>
#include <core/component/children.hpp>
#include <core/component/sprite.hpp>
#include <core/tag/input.hpp>
#include <core/event/del_entity.hpp>
#include <survivor/manager/achievement.hpp>
#include <survivor/component/hp.hpp>
#include <survivor/event/hp_change.hpp>
#include <survivor/event/death.hpp>
#include <survivor/tag/hp_bar.hpp>

using namespace core;

namespace survivor {

EventSystem::EventSystem() : BaseSystem("Event") {
}

void EventSystem::update_impl() {
    if (player_entity_ == wheel::NullEntity) {
        player_entity_ = ecs.get_entity<InputTag>();
    }
    hp_change_event_();
    death_event_();
}

void EventSystem::hp_change_event_() {
    for (auto [source, target, value] : ecs.get_events<HPChangeEvent>()) {
        if (ecs.has_component<HPComponent>(target)) {
            // update HP
            auto& hp = ecs.get_component<HPComponent>(target);
            hp.hp += value;
            if (hp.hp <= 0) {
                ecs.emplace_event<DeathEvent>(source, target);
            }

            // update HP bar
            for (auto child : ecs.get_component<ChildrenComponent>(target).children) {
                if (ecs.has_components<HPBarTag, SpriteComponent>(child)) {
                    auto bar_id = std::clamp(static_cast<int>(std::round(48.f * hp.hp / hp.max_hp)), 0, 48);
                    ecs.get_component<SpriteComponent>(child).sprite = &SpriteManager::instance().get("hp_bar" + std::to_string(bar_id));
                }
            }
        }
    }
}

void EventSystem::death_event_() {
    for (auto [source, target] : ecs.get_events<DeathEvent>()) {
        if (source == player_entity_) {
            AchievementManager::instance().add_kill();
        }
        if (ecs.has_entity(target)) {
            ecs.emplace_event<DelEntityEvent>(target);
        }
    }
}

}  // namespace survivor
