#include <core/manager/enemy.hpp>
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
#include <core/component/hp.hpp>
#include <core/component/fraction.hpp>
#include <core/component/loot.hpp>
#include <core/tag/input.hpp>
#include <core/tag/rigidbody.hpp>
#include <core/tag/obstacle.hpp>

#include <wheel/random.hpp>

#include <rfl/json.hpp>

using namespace core;

namespace core {

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
}

wheel::Entity EnemyManager::generate(
    const std::string& name,
    wheel::Vector2D<float> position
) const {
    const auto& config = enemy_configs_.at(name);

    return EntityManager::instance().add_entity(
        NameComponent{name},
        TransformComponent{position, {64.f, 64.f}},
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
}

}  // namespace core
