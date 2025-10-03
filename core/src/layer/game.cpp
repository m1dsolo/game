#include <core/layer/game.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/game.hpp>
#include <core/manager/ui.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/sprite.hpp>
#include <core/component/render.hpp>
#include <core/component/hp.hpp>
#include <core/tag/hp_bar.hpp>
#include <core/tag/rigidbody.hpp>
#include <core/tag/obstacle.hpp>
#include <core/system/move.hpp>

#include <sdl/sdl.hpp>

namespace core {

void GameLayer::on_attach() {
    // init hp bar sprites
    for (int i = 1; i <= 47; i++) {
        auto texture = sdl::SDL::create_texture(48, 12, sdl::SDL::RED);
        auto target = sdl::SDL::RenderTargetGuard{texture};
        auto dst = SDL_FRect{0.f, 0.f, static_cast<float>(i), 12.f};
        sdl::SDL::render_filled_rect(&dst, sdl::SDL::GREEN);
        SpriteManager::instance().set("hp_bar" + std::to_string(i), {
            texture,
            {0.f, 0.f, 48.f, 12.f}
        });
    }
    // hidden hp bar when hp is empty or full
    SpriteManager::instance().set("hp_bar0", {
        nullptr,
        {0.f, 0.f, 48.f, 12.f}
    });
    SpriteManager::instance().set("hp_bar48", {
        nullptr,
        {0.f, 0.f, 48.f, 12.f}
    });

    auto& entity_manager = EntityManager::instance();

    // set add hp bar callback
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

    // init map
    float map_width = context.map_width;
    float map_height = context.map_height;
    SpriteManager::instance().set("map", Sprite{
        sdl::SDL::create_texture(map_width, map_height, SDL_FColor{204.f / 255.f, 1.f, 153.f / 255.f, 1.f}),
        {0.f, 0.f, map_width, map_height}
    });
    auto map = entity_manager.add_entity(
        NameComponent{"map"},
        TransformComponent{{0.f, 0.f}, {map_width, map_height}},
        SpriteComponent{"map"},
        RenderComponent{0}
    );

    // init boundaries
    const float thickness = 40.f;
    auto left_boundary = entity_manager.add_entity(
        NameComponent{"left_boundary"},
        TransformComponent{{-map_width / 2 - thickness / 2, 0.f}, {thickness, map_height + thickness * 2}},
        ColliderComponent{
            wheel::Rect<float>{{0.f, 0.f}, {thickness, map_height + thickness * 2}},
            ColliderLayer::Obstacle,
            ColliderLayer::Player | ColliderLayer::Enemy
        },
        RigidbodyTag{},
        // SpriteComponent{sdl::SDL::RED},
        // RenderComponent{4},
        ObstacleTag{}
    );
    auto right_boundary = entity_manager.add_entity(
        NameComponent{"right_boundary"},
        TransformComponent{{map_width / 2 + thickness / 2, 0.f}, {thickness, map_height + thickness * 2}},
        ColliderComponent{
            wheel::Rect<float>{{0.f, 0.f}, {thickness, map_height + thickness * 2}},
            ColliderLayer::Obstacle,
            ColliderLayer::Player | ColliderLayer::Enemy
        },
        RigidbodyTag{},
        // SpriteComponent{sdl::SDL::RED},
        // RenderComponent{4},
        ObstacleTag{}
    );
    auto top_boundary = entity_manager.add_entity(
        NameComponent{"top_boundary"},
        TransformComponent{{0.f, -map_height / 2 - thickness / 2}, {map_width + thickness * 2, thickness}},
        ColliderComponent{
            wheel::Rect<float>{{0.f, 0.f}, {map_width + thickness * 2, thickness}},
            ColliderLayer::Obstacle,
            ColliderLayer::Player | ColliderLayer::Enemy
        },
        RigidbodyTag{},
        // SpriteComponent{sdl::SDL::RED},
        // RenderComponent{4},
        ObstacleTag{}
    );
    auto bottom_boundary = entity_manager.add_entity(
        NameComponent{"bottom_boundary"},
        TransformComponent{{0.f, map_height / 2 + thickness / 2}, {map_width + thickness * 2, thickness}},
        ColliderComponent{
            wheel::Rect<float>{wheel::Rect<float>{{0.f, 0.f}, {map_width + thickness * 2, thickness}}},
            ColliderLayer::Obstacle,
            ColliderLayer::Player | ColliderLayer::Enemy
        },
        RigidbodyTag{},
        // SpriteComponent{sdl::SDL::RED},
        // RenderComponent{4},
        ObstacleTag{}
    );

    GameManager::instance().start();
}

void GameLayer::on_detach() {
    // TODO: clean

    GameManager::instance().stop();
}

bool GameLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_W: MoveSystem::is_move_up = true; return true;
                case SDLK_S: MoveSystem::is_move_down = true; return true;
                case SDLK_A: MoveSystem::is_move_left = true; return true;
                case SDLK_D: MoveSystem::is_move_right = true; return true;
                case SDLK_ESCAPE: {
                    GameManager::instance().pause();
                    return true;
                }
            }
            break;
        }
        case SDL_EVENT_KEY_UP: {
            switch (event.key.key) {
                case SDLK_W: MoveSystem::is_move_up = false; return true;
                case SDLK_S: MoveSystem::is_move_down = false; return true;
                case SDLK_A: MoveSystem::is_move_left = false; return true;
                case SDLK_D: MoveSystem::is_move_right = false; return true;
            }
            break;
        }
        // case SDL_Event_GAMEPAD_ADDED
        case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
            const auto& gaxis = event.gaxis;
            switch (gaxis.axis) {
                case SDL_GAMEPAD_AXIS_LEFTX: {
                    float value = gaxis.value / 32767.0f;
                    MoveSystem::is_move_left = (value < -0.25);
                    MoveSystem::is_move_right = (value > 0.25);
                    return true;
                }
                case SDL_GAMEPAD_AXIS_LEFTY: {
                    float value = gaxis.value / 32767.0f;
                    MoveSystem::is_move_up = (value < -0.25);
                    MoveSystem::is_move_down = (value > 0.25);
                    return true;
                }
            }
            break;
        }
    }

    return false;
}

}  // namespace core
