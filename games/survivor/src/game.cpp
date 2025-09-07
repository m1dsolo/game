#include <survivor/game.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/system.hpp>
#include <core/manager/sprite.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/sprite.hpp>
#include <core/component/direction.hpp>
#include <core/component/speed.hpp>
#include <core/component/collider.hpp>
#include <core/component/animation.hpp>
#include <core/component/render.hpp>
#include <core/tag/input.hpp>
#include <core/system/sdl_event.hpp>
#include <core/system/move.hpp>
#include <core/system/transform.hpp>
#include <core/system/animation.hpp>
#include <core/system/render.hpp>
#include <core/system/time.hpp>
#include <survivor/component/hp.hpp>
#include <survivor/tag/hp_bar.hpp>

namespace survivor {

SurvivorGame::SurvivorGame() {
    core::SystemManager::instance().add_systems<
        core::SDLEventSystem,
        core::MoveSystem,
        core::TransformSystem,
        core::AnimationSystem,
        core::RenderSystem,
        core::TimeSystem
    >(true);

    for (int i = 0; i <= 48; i++) {
        auto texture = sdl::SDL::create_texture(48, 12, sdl::SDL::RED);
        auto target = sdl::SDL::TargetGuard{texture};
        auto dst = SDL_FRect{0.f, 0.f, static_cast<float>(i), 12.f};
        sdl::SDL::render_fill_rect(&dst, sdl::SDL::GREEN);
        core::SpriteManager::instance().set("health_bar" + std::to_string(i), {
            texture,
            {0.f, 0.f, 48.f, 12.f}
        });
    }

    auto& entity_manager = core::EntityManager::instance();
    entity_manager.set_add_entity_callback([](wheel::Entity entity) {
        if (core::ecs.has_component<HPComponent>(entity)) {
            core::EntityManager::instance().add_entity(
                entity,
                core::NameComponent{"health_bar"},
                core::TransformComponent{{0.f, -36.f}, {48.f, 12.f}},
                core::SpriteComponent{"health_bar48"},
                core::RenderComponent{2},
                HPBarTag{}
            );
        }
    });

    auto slime = entity_manager.add_entity(
        core::NameComponent{"slime"},
        core::TransformComponent{{0.f, 0.f}, {48.f, 48.f}},
        core::SpriteComponent{},
        core::DirectionComponent{},
        core::SpeedComponent{200.f},
        core::ColliderComponent{{24.f, 24.f}},
        core::AnimationComponent{"slime-idle-down"},
        core::RenderComponent{1},
        core::InputTag{},
        HPComponent{100}
    );

    auto skeleton = entity_manager.add_entity(
        core::NameComponent{"skeleton"},
        core::TransformComponent{{0.f, 100.f}, {48.f, 48.f}},
        core::SpriteComponent{},
        core::DirectionComponent{},
        core::SpeedComponent{200.f},
        core::ColliderComponent{{24.f, 24.f}},
        core::AnimationComponent{"skeleton-idle-down"},
        core::RenderComponent{1},
        HPComponent{100}
    );

    auto house = entity_manager.add_entity(
        core::NameComponent{"house"},
        core::TransformComponent{{100.f, 0.f}, {48.f, 48.f}},
        core::SpriteComponent{"house"},
        core::ColliderComponent{{48.f, 48.f}, false},
        core::RenderComponent{0}
    );

    core::SpriteManager::instance().set("map", core::Sprite{
        sdl::SDL::create_texture(800, 600, sdl::SDL::WHITE),
        {0.f, 0.f, 800.f, 600.f}
    });
    auto map = entity_manager.add_entity(
        core::NameComponent{"map"},
        core::TransformComponent{{0.f, 0.f}, {800.f, 600.f}},
        core::SpriteComponent{"map"},
        core::RenderComponent{0}
    );
}

}  // namespace survivor
