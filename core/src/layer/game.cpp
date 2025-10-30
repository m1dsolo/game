#include <core/layer/game.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/system.hpp>
#include <core/manager/sprite.hpp>
#include <core/manager/save.hpp>
#include <core/layer/inventory.hpp>
#include <core/layer/pause_menu.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/collider.hpp>
#include <core/component/sprite.hpp>
#include <core/component/render.hpp>
#include <core/component/hp.hpp>
#include <core/component/item.hpp>
#include <core/tag/root.hpp>
#include <core/tag/camera.hpp>
#include <core/tag/rigidbody.hpp>
#include <core/tag/obstacle.hpp>
#include <core/tag/render.hpp>
#include <core/tag/map.hpp>
#include <core/tag/game_layer.hpp>
#include <core/resource/context.hpp>
#include <core/resource/input.hpp>
#include <core/event/layer.hpp>
#include <core/entity_event/remove_entity.hpp>

#include <sdl/sdl.hpp>

namespace core {

void init_map_();

void GameLayer::on_attach() {
    if (!new_game_requested) {
        SaveManager::instance().load();
    }
    init_map_();
}

void GameLayer::on_detach() {
    auto entities = ecs.get_entities<GameLayerTag>() | std::ranges::to<std::vector>();
    auto& entity_manager = EntityManager::instance();
    for (auto entity : entities) {
        entity_manager.remove_entity(entity);
    }
}

void GameLayer::on_show() {
    SystemManager::instance().resume_game_systems();
}

void GameLayer::on_hide() {
    SystemManager::instance().pause_game_systems();
}

bool GameLayer::on_event(const SDL_Event& event) {
    auto& input = ecs.get_resource<InputResource>();
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_W: input.is_move_up = true; return true;
                case SDLK_S: input.is_move_down = true; return true;
                case SDLK_A: input.is_move_left = true; return true;
                case SDLK_D: input.is_move_right = true; return true;
                case SDLK_I: {
                    ecs.emplace_event<AddLayerEvent>("InventoryLayer");
                    return true;
                }
                case SDLK_ESCAPE: {
                    ecs.emplace_event<AddLayerEvent>("PauseMenuLayer");
                    return true;
                }
            }
            break;
        }
        case SDL_EVENT_KEY_UP: {
            switch (event.key.key) {
                case SDLK_W: input.is_move_up = false; return true;
                case SDLK_S: input.is_move_down = false; return true;
                case SDLK_A: input.is_move_left = false; return true;
                case SDLK_D: input.is_move_right = false; return true;
            }
            break;
        }
        case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
            const auto& gaxis = event.gaxis;
            switch (gaxis.axis) {
                case SDL_GAMEPAD_AXIS_LEFTX: {
                    float value = gaxis.value / 32767.0f;
                    input.is_move_left = (value < -0.25);
                    input.is_move_right = (value > 0.25);
                    return true;
                }
                case SDL_GAMEPAD_AXIS_LEFTY: {
                    float value = gaxis.value / 32767.0f;
                    input.is_move_up = (value < -0.25);
                    input.is_move_down = (value > 0.25);
                    return true;
                }
            }
            break;
        }
    }

    return false;
}

void init_map_() {
    auto& entity_manager = EntityManager::instance();

    // init map
    const auto& context = ecs.get_resource<ContextResource>();
    float map_width = context.map_width;
    float map_height = context.map_height;
    auto map = entity_manager.add_entity(
        NameComponent{"map"},
        TransformComponent{{{0.f, 0.f}, {map_width, map_height}}},
        SpriteComponent{"map"},
        RenderComponent{0},
        RenderTag{},
        MapTag{},
        GameLayerTag{}
    );

    // init boundaries
    const float thickness = 40.f;
    auto left_boundary = entity_manager.add_entity(
        NameComponent{"left_boundary"},
        TransformComponent{{{-map_width / 2 - thickness / 2, 0.f}, {thickness, map_height + thickness * 2}}},
        ColliderComponent{
            wheel::Rect<float>{{0.f, 0.f}, {thickness, map_height + thickness * 2}},
            ColliderLayer::Obstacle,
            ColliderLayer::Player | ColliderLayer::Enemy
        },
        RigidbodyTag{},
        ObstacleTag{},
        MapTag{},
        GameLayerTag{}
    );
    auto right_boundary = entity_manager.add_entity(
        NameComponent{"right_boundary"},
        TransformComponent{{{map_width / 2 + thickness / 2, 0.f}, {thickness, map_height + thickness * 2}}},
        ColliderComponent{
            wheel::Rect<float>{{0.f, 0.f}, {thickness, map_height + thickness * 2}},
            ColliderLayer::Obstacle,
            ColliderLayer::Player | ColliderLayer::Enemy
        },
        RigidbodyTag{},
        ObstacleTag{},
        MapTag{},
        GameLayerTag{}
    );
    auto top_boundary = entity_manager.add_entity(
        NameComponent{"top_boundary"},
        TransformComponent{{{0.f, -map_height / 2 - thickness / 2}, {map_width + thickness * 2, thickness}}},
        ColliderComponent{
            wheel::Rect<float>{{0.f, 0.f}, {map_width + thickness * 2, thickness}},
            ColliderLayer::Obstacle,
            ColliderLayer::Player | ColliderLayer::Enemy
        },
        RigidbodyTag{},
        ObstacleTag{},
        MapTag{},
        GameLayerTag{}
    );
    auto bottom_boundary = entity_manager.add_entity(
        NameComponent{"bottom_boundary"},
        TransformComponent{{{0.f, map_height / 2 + thickness / 2}, {map_width + thickness * 2, thickness}}},
        ColliderComponent{
            wheel::Rect<float>{wheel::Rect<float>{{0.f, 0.f}, {map_width + thickness * 2, thickness}}},
            ColliderLayer::Obstacle,
            ColliderLayer::Player | ColliderLayer::Enemy
        },
        RigidbodyTag{},
        ObstacleTag{},
        MapTag{},
        GameLayerTag{}
    );
}

}  // namespace core
