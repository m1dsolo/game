#include <survivor/system/hp_change.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/sprite.hpp>
#include <core/manager/time.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/direction.hpp>
#include <core/component/speed.hpp>
#include <core/component/children.hpp>
#include <core/component/sprite.hpp>
#include <core/component/render.hpp>
#include <core/tag/input.hpp>
#include <core/event/del_entity.hpp>
#include <survivor/manager/achievement.hpp>
#include <survivor/component/hp.hpp>
#include <survivor/component/loot.hpp>
#include <survivor/component/item.hpp>
#include <survivor/component/inventory.hpp>
#include <survivor/event/hp_change.hpp>
#include <survivor/event/death.hpp>
#include <survivor/tag/hp_bar.hpp>

#include <wheel/random.hpp>

using namespace core;

namespace survivor {

void hp_change_event_();
void death_event_();

void HPChangeSystem::operator()() {
    hp_change_event_();
    death_event_();
}

void hp_change_event_() {
    for (auto [source, target, value] : ecs.get_events<HPChangeEvent>()) {
        if (ecs.has_component<HPComponent>(target)) {
            auto& hp = ecs.get_component<HPComponent>(target);
            if (hp.hp <= 0) {
                continue;
            }

            // update HP
            hp.hp += value;
            if (hp.hp <= 0) {
                ecs.emplace_event<DeathEvent>(source, target);
            }

            // generate HP change float text
            const auto& target_transform = ecs.get_component<TransformComponent>(target);
            auto& sprite_manager = SpriteManager::instance();
            auto key = "hp_change" + std::to_string(value);
            if (!sprite_manager.has(key)) {
                auto texture = sdl::SDL::create_texture(std::to_string(std::abs(value)), 20.f, value > 0 ? sdl::SDL::GREEN : sdl::SDL::RED);
                sprite_manager.set(key, Sprite{texture});
            }
            auto [w, h] = sdl::SDL::get_texture_size(sprite_manager.get(key).texture);
            auto position = target_transform.global.position;
            auto size = target_transform.global.size;
            auto text_entity = EntityManager::instance().add_entity(
                TransformComponent{
                    position - wheel::Vector2D<float>{0.f, size[1] / 2.f + 10.f},
                    {w, h}
                },
                DirectionComponent{{wheel::Random::instance().uniform<float>(-0.5f, 0.5f), -1.f}},
                SpeedComponent{wheel::Random::instance().uniform<float>(10.f, 20.f)},
                SpriteComponent{key},
                RenderComponent{1}
            );
            TimeManager::instance().timer().add(1000000, [text_entity]() {
                if (ecs.has_entity(text_entity)) {
                    ecs.emplace_event<DelEntityEvent>(text_entity);
                }
                return 0;
            });

            // update HP bar
            for (auto child : ecs.get_component<ChildrenComponent>(target).children) {
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

void death_event_() {
    auto player_entity = ecs.get_entity<InputTag>();
    for (auto [source, target] : ecs.get_events<DeathEvent>()) {
        if (source == player_entity) {
            std::cout << "source: " << ecs.get_component<NameComponent>(source).name
                      << " killed target: " << ecs.get_component<NameComponent>(target).name << " " << target << std::endl;
            AchievementManager::instance().add_kill();
        }
        if (ecs.has_entity(target)) {
            ecs.emplace_event<DelEntityEvent>(target);
        }

        if (ecs.has_component<LootComponent>(target)) {
            for (const auto& group : ecs.get_component<LootComponent>(target).groups) {
                if (wheel::Random::instance().uniform<float>(0.f, 1.f) <= group.chance) {
                    int total_weight = 0;
                    for (const auto& item : group.items) {
                        total_weight += item.weight;
                    }
                    int pick = wheel::Random::instance().uniform(1, total_weight);
                    for (const auto& item : group.items) {
                        pick -= item.weight;
                        if (pick <= 0) {
                            int count = wheel::Random::instance().uniform(item.count.first, item.count.second);
                            EntityManager::instance().add_entity(
                                NameComponent{item.name},
                                TransformComponent{
                                    ecs.get_component<TransformComponent>(target).global.position,
                                    {16.f, 16.f}
                                },
                                SpriteComponent{item.name},
                                ColliderComponent{wheel::Rect<float>{{0.f, 0.f}, {16.f, 16.f}}},
                                RenderComponent{1},
                                ItemComponent{item.name, count}
                            );
                            break;
                        }
                    }
                }
            }
        }
    }
}

}  // namespace survivor
