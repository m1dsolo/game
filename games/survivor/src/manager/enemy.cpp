#include <survivor/manager/enemy.hpp>
#include <survivor/component/hp.hpp>
#include <survivor/component/fraction.hpp>
#include <survivor/component/loot.hpp>
#include <survivor/tag/obstacle.hpp>
#include <survivor/collider_layer.hpp>

#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/time.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/sprite.hpp>
#include <core/component/direction.hpp>
#include <core/component/speed.hpp>
#include <core/component/collider.hpp>
#include <core/component/animation.hpp>
#include <core/component/animation_fsm.hpp>
#include <core/component/render.hpp>
#include <core/component/track.hpp>
#include <core/tag/input.hpp>
#include <core/tag/rigidbody.hpp>

#include <wheel/random.hpp>

#include <rfl/json.hpp>

#include <fstream>

using namespace core;

namespace survivor {

wheel::Vector2D<float> random_pick_point(const wheel::Rect<float>& rect);

EnemyManager::EnemyManager() {
    auto& entity_manager = EntityManager::instance();

    std::ifstream file("assets/config/enemy.json");
    if (file.is_open()) {
        std::cout << "[begin load enemy...]" << std::endl;
        auto result = rfl::json::read<std::vector<EnemyConfig>>(file);
        const auto enemies = result.value();
        for (const auto& enemy_config : enemies) {
            enemy_configs_[enemy_config.name] = enemy_config;
        }
        std::cout << "[load enemy done]" << std::endl;
    }

    float map_width = config.map_width;
    float map_height = config.map_height;

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

    boundaries_ = { left_boundary, right_boundary, top_boundary, bottom_boundary };
}

void EnemyManager::generate_waves() const {
    TimeManager::instance().timer().add(1000000, [this]() {
        generate("thief");

        return 1000000;
    });
}

wheel::Entity EnemyManager::generate(const std::string& name) const {
    auto boundary = boundaries_[wheel::Random::instance().uniform(0, static_cast<int>(boundaries_.size()) - 1)];
    const auto& transform = ecs.get_component<TransformComponent>(boundary).global;
    auto pos = random_pick_point(wheel::Rect<float>{transform.position, transform.size});

    const auto& config = enemy_configs_.at(name);
    EntityManager::instance().add_entity(
        NameComponent{name},
        TransformComponent{pos, {64.f, 64.f}},
        SpriteComponent{},
        DirectionComponent{},
        SpeedComponent{config.speed},
        ColliderComponent{
            wheel::Rect<float>{{0.f, 0.f}, {32.f, 32.f}},
            ColliderLayer::Enemy,
            ColliderLayer::Player
        },
        RigidbodyTag{},
        AnimationComponent{{name}},
        AnimationFSMComponent{"basic"},
        RenderComponent{1},
        TrackComponent{ecs.get_entity<InputTag>()},
        HPComponent{config.hp},
        FractionComponent{1},
        LootComponent{config.loots}
    );

    return 1000000;
}

wheel::Vector2D<float> random_pick_point(const wheel::Rect<float>& rect) {
    auto& random = wheel::Random::instance();
    return {
        random.uniform(rect.x0, rect.x1),
        random.uniform(rect.y0, rect.y1)
    };
}

}  // namespace survivor
