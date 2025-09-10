#include <survivor/game.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/system.hpp>
#include <core/manager/sprite.hpp>
#include <core/manager/time.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/sprite.hpp>
#include <core/component/direction.hpp>
#include <core/component/speed.hpp>
#include <core/component/collider.hpp>
#include <core/component/animation.hpp>
#include <core/component/render.hpp>
#include <core/component/track.hpp>
#include <core/component/trigger.hpp>
#include <core/component/data.hpp>
#include <core/tag/input.hpp>
#include <core/system/sdl_event.hpp>
#include <core/system/track.hpp>
#include <core/system/move.hpp>
#include <core/system/trigger.hpp>
#include <core/system/transform.hpp>
#include <core/system/animation.hpp>
#include <core/system/render.hpp>
#include <core/system/time.hpp>
#include <core/system/del_entity_event.hpp>
#include <survivor/system/event.hpp>
#include <survivor/component/hp.hpp>
#include <survivor/tag/hp_bar.hpp>
#include <survivor/event/hp_change.hpp>

using namespace core;

namespace survivor {

SurvivorGame::SurvivorGame() {
    SystemManager::instance().add_systems<
        SDLEventSystem,
        TrackSystem,
        MoveSystem,
        TriggerSystem,
        TransformSystem,
        AnimationSystem,
        RenderSystem,
        TimeSystem,
        EventSystem,
        DelEntityEventSystem
    >(true);

    for (int i = 0; i <= 48; i++) {
        auto texture = sdl::SDL::create_texture(48, 12, sdl::SDL::RED);
        auto target = sdl::SDL::TargetGuard{texture};
        auto dst = SDL_FRect{0.f, 0.f, static_cast<float>(i), 12.f};
        sdl::SDL::render_fill_rect(&dst, sdl::SDL::GREEN);
        SpriteManager::instance().set("hp_bar" + std::to_string(i), {
            texture,
            {0.f, 0.f, 48.f, 12.f}
        });
    }

    auto& entity_manager = EntityManager::instance();
    entity_manager.set_add_entity_callback([](wheel::Entity entity) {
        if (ecs.has_component<HPComponent>(entity)) {
            EntityManager::instance().add_entity(
                entity,
                NameComponent{"hp_bar"},
                TransformComponent{{0.f, -36.f}, {48.f, 12.f}},
                SpriteComponent{"hp_bar48"},
                RenderComponent{2},
                HPBarTag{}
            );
        }
    });

    auto slime = entity_manager.add_entity(
        NameComponent{"slime"},
        TransformComponent{{0.f, 0.f}, {48.f, 48.f}},
        SpriteComponent{},
        DirectionComponent{},
        SpeedComponent{200.f},
        ColliderComponent{{24.f, 24.f}},
        AnimationComponent{"slime-idle-down"},
        RenderComponent{1},
        InputTag{},
        HPComponent{100}
    );
    auto damage_aura = entity_manager.add_entity(
        slime,
        NameComponent{"damage_aura"},
        TransformComponent{{0.f, 0.f}},
        DataComponent{{{"entity2timer_id", std::unordered_map<wheel::Entity, wheel::timer_id_t>{}}}},
        TriggerComponent{
            {100.f, 100.f},
            true,
            [](wheel::Entity entity, wheel::Entity other) {
                if (!ecs.get_component<TriggerComponent>(entity).stay_entities.contains(other)
                        && ecs.has_component<HPComponent>(other)) {
                    std::cout << ecs.get_component<NameComponent>(other).name << ecs.get_component<HPComponent>(other).hp << std::endl;;
                    auto timer_id = TimeManager::instance().timer().add(1000000, [entity, other]() {
                        if (!ecs.has_component<TriggerComponent>(entity)) {
                            return 0;
                        }
                        if (!ecs.get_component<TriggerComponent>(entity).stay_entities.contains(other)) {
                            return 0;
                        }

                        if (ecs.has_component<HPComponent>(other)) {
                            ecs.emplace_event<HPChangeEvent>(entity, other, -10);
                        }
                        return 1000000;
                    });
                    auto& entity2timer_id = std::any_cast<std::unordered_map<wheel::Entity, wheel::timer_id_t>&>(
                        ecs.get_component<DataComponent>(entity).data["entity2timer_id"]
                    );
                    entity2timer_id[other] = timer_id;
                }
            },
            [](wheel::Entity entity, wheel::Entity other) {
            },
            [](wheel::Entity entity, wheel::Entity other) {
                auto& entity2timer_id = std::any_cast<std::unordered_map<wheel::Entity, wheel::timer_id_t>&>(
                    ecs.get_component<DataComponent>(entity).data["entity2timer_id"]
                );
                if (auto iter = entity2timer_id.find(other); iter != entity2timer_id.end()) {
                    TimeManager::instance().timer().remove(iter->second);
                    entity2timer_id.erase(iter);
                }
            }
        }
    );

    auto skeleton = entity_manager.add_entity(
        NameComponent{"skeleton"},
        TransformComponent{{0.f, 100.f}, {48.f, 48.f}},
        SpriteComponent{},
        DirectionComponent{},
        SpeedComponent{150.f},
        ColliderComponent{{24.f, 24.f}},
        AnimationComponent{"skeleton-idle-down"},
        RenderComponent{1},
        TrackComponent{slime},
        HPComponent{100}
    );

    auto house = entity_manager.add_entity(
        NameComponent{"house"},
        TransformComponent{{100.f, 0.f}, {48.f, 48.f}},
        SpriteComponent{"house"},
        ColliderComponent{{48.f, 48.f}, false},
        RenderComponent{0}
    );

    SpriteManager::instance().set("map", Sprite{
        sdl::SDL::create_texture(800, 600, sdl::SDL::WHITE),
        {0.f, 0.f, 800.f, 600.f}
    });
    auto map = entity_manager.add_entity(
        NameComponent{"map"},
        TransformComponent{{0.f, 0.f}, {800.f, 600.f}},
        SpriteComponent{"map"},
        RenderComponent{0}
    );
}

}  // namespace survivor
