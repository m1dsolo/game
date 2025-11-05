#include <core/manager/enemy.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
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
#include <core/component/attack.hpp>
#include <core/component/loot.hpp>
#include <core/component/master.hpp>
#include <core/tag/input.hpp>
#include <core/tag/rigidbody.hpp>
#include <core/tag/render.hpp>
#include <core/tag/save.hpp>
#include <core/tag/game_layer.hpp>

#include <wheel/random.hpp>

#include <rfl/json.hpp>

using namespace core;

namespace core {

EnemyManager::EnemyManager() {
    auto& entity_manager = EntityManager::instance();

    std::ifstream file("assets/config/enemy.json");
    if (file.is_open()) {
        std::cout << "[begin load enemy...]" << std::endl;
        auto result = rfl::json::read<std::vector<EnemyConfig>, rfl::DefaultIfMissing>(file);
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
    auto& entity_manager = EntityManager::instance();
    const auto& config = enemy_configs_.at(name);

    auto entity = entity_manager.add_entity(
        NameComponent{name},
        TransformComponent{{{position}, {64.f, 64.f}}},
        SpriteComponent{},
        DirectionComponent{},
        SpeedComponent{config.speed},
        ColliderComponent{
            wheel::Rect<float>{{0.f, 0.f}, {32.f, 32.f}},
            ColliderLayer::Enemy,
            ColliderLayer::Player
        },
        AnimationComponent{{name}},
        AnimationFSMComponent{"basic"},
        RenderComponent{5},
        TrackComponent{ecs.get_entity<InputTag>()},
        HPComponent{config.hp, config.hp},
        LootComponent{config.loots},
        RigidbodyTag{},
        RenderTag{},
        SaveTag{},
        GameLayerTag{}
    );
    auto [damage, range, interval] = config.attack;
    auto attack = entity_manager.add_entity(
        entity,
        NameComponent{"attack"},
        TransformComponent{},
        ColliderComponent{
            wheel::Circle<float>{range},
            ColliderLayer::Enemy,
            ColliderLayer::Player
        },
        AttackComponent{damage, interval},
        MasterComponent{entity},
        SaveTag{},
        GameLayerTag{}
    );

    return entity;
}

}  // namespace core
