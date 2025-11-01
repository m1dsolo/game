#include <survivor/layer/game.hpp>
#include <survivor/manager/wave.hpp>
#include <survivor/tag/hud.hpp>
#include <survivor/resource/achievement.hpp>

#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/layer.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/collider.hpp>
#include <core/component/direction.hpp>
#include <core/component/speed.hpp>
#include <core/component/sprite.hpp>
#include <core/component/animation.hpp>
#include <core/component/animation_fsm.hpp>
#include <core/component/render.hpp>
#include <core/component/hp.hpp>
#include <core/component/level.hpp>
#include <core/component/item.hpp>
#include <core/component/inventory.hpp>
#include <core/component/master.hpp>
#include <core/component/aura_damage.hpp>
#include <core/component/range_attack.hpp>
#include <core/component/reload.hpp>
#include <core/component/track.hpp>
#include <core/component/text.hpp>
#include <core/tag/camera.hpp>
#include <core/tag/rigidbody.hpp>
#include <core/tag/render.hpp>
#include <core/tag/input.hpp>
#include <core/tag/pickup_item.hpp>
#include <core/tag/absorb_item.hpp>
#include <core/tag/save.hpp>
#include <core/tag/game_layer.hpp>
#include <core/resource/context.hpp>
#include <core/resource/inventory.hpp>
#include <core/sdl_user_event/update_slot.hpp>

using namespace core;

namespace survivor {

void new_game();

void GameLayer::on_attach() {
    core::GameLayer::on_attach();

    if (new_game_requested) {
        new_game();
        auto camera = ecs.get_entity<CameraTag>();
        auto player = ecs.get_entity<InputTag>();
        Hierarchy::attach_entity_to_parent(camera, player);
    }

    WaveManager::instance().start_generate_waves();
    ecs.add_resource(AchievementResource{});
}

void GameLayer::on_detach() {
    WaveManager::instance().stop_generate_waves();

    auto camera = ecs.get_entity<CameraTag>();
    Hierarchy::attach_entity_to_parent(camera, Hierarchy::root());

    core::GameLayer::on_detach();
}

void new_game() {
    const auto& context = ecs.get_resource<ContextResource>();
    const auto& slot_nums = ecs.get_resource<InventoryResource>().slot_nums;

    auto& entity_manager = EntityManager::instance();

    auto bunny = entity_manager.add_entity(
        NameComponent{"bunny"},
        TransformComponent{{{0.f, 0.f}, {64.f, 64.f}}},
        DirectionComponent{},
        SpeedComponent{200.f},
        ColliderComponent{
            wheel::Rect<float>{{0.f, 0.f}, {16.f, 16.f}},
            ColliderLayer::Player,
            ColliderLayer::Enemy | ColliderLayer::Obstacle
        },
        SpriteComponent{},
        AnimationComponent{{"bunny"}},
        AnimationFSMComponent{"basic"},
        RenderComponent{1},
        HPComponent{100, 100},
        LevelComponent{},
        InventoryComponent{},
        RigidbodyTag{},
        RenderTag{},
        InputTag{},
        PickupItemTag{},
        SaveTag{},
        GameLayerTag{}
    );

    auto& inventory = ecs.get_component<InventoryComponent>();
    inventory.slots.resize(slot_nums[0] * slot_nums[1]);
    inventory.slots[0] = {"usp", 2};

    entity_manager.add_entity(
        bunny,
        NameComponent{"absorb"},
        TransformComponent{},
        ColliderComponent{
            wheel::Circle<float>{200.f},
            ColliderLayer::Trigger,
            ColliderLayer::Item
        },
        AbsorbItemTag{},
        MasterComponent{bunny},
        SaveTag{},
        GameLayerTag{}
    );

    auto damage_aura = entity_manager.add_entity(
        bunny,
        NameComponent{"damage_aura"},
        TransformComponent{{{0.f, 0.f}, {300.f, 300.f}}},
        SpriteComponent{"damage_aura"},
        RenderComponent{2},
        ColliderComponent{
            wheel::Circle<float>{150.f},
            ColliderLayer::Trigger,
            ColliderLayer::Enemy
        },
        AuraDamageComponent{8, 500000},
        MasterComponent{bunny},
        RenderTag{},
        SaveTag{},
        GameLayerTag{}
    );

    auto auto_shoot = entity_manager.add_entity(
        bunny,
        NameComponent("auto_shoot"),
        TransformComponent{{{0.f, 0.f}, {500.f, 500.f}}},
        ColliderComponent{
            wheel::Circle<float>{250.f},
            ColliderLayer::Trigger,
            ColliderLayer::Enemy
        },
        RangeAttackComponent{
            .damage = 7,
            .interval = 200000,
            .projectile_speed = 500.f,
            .projectile_sprite_id = "bullet",
            .range_attack_sound_id = "m4a1/shoot"
        },
        ReloadComponent{30, 30, 2000000, "m4a1/reload"},
        MasterComponent{bunny},
        SpriteComponent{"auto_shoot"},
        RenderComponent{3},
        RenderTag{},
        SaveTag{},
        GameLayerTag{}
    );

    auto fox = entity_manager.add_entity(
        NameComponent{"fox"},
        TransformComponent{{{50.f, 0.f}, {64.f, 64.f}}},
        DirectionComponent{},
        SpeedComponent{150.f},
        ColliderComponent{
            wheel::Rect<float>{{0.f, 0.f}, {16.f, 16.f}},
            ColliderLayer::Player,
            ColliderLayer::Enemy | ColliderLayer::Obstacle
        },
        SpriteComponent{},
        AnimationComponent{{"fox"}},
        AnimationFSMComponent{"basic"},
        RenderComponent{5},
        HPComponent{100, 100},
        LevelComponent{},
        TrackComponent{bunny, 50.f},
        RigidbodyTag{},
        RenderTag{},
        SaveTag{},
        GameLayerTag{}
    );
    auto fox_auto_shoot = entity_manager.add_entity(
        fox,
        NameComponent("fox_auto_shoot"),
        TransformComponent{{{0.f, 0.f}, {300.f, 300.f}}},
        ColliderComponent{
            wheel::Circle<float>{150.f},
            ColliderLayer::Trigger,
            ColliderLayer::Enemy
        },
        RangeAttackComponent{
            .damage = 10,
            .interval = 500000,
            .projectile_speed = 500.f,
            .projectile_sprite_id = "bullet",
            .range_attack_sound_id = "usp/shoot"
        },
        ReloadComponent{12, 12, 1000000, "usp/reload"},
        MasterComponent{bunny},
        SpriteComponent{"auto_shoot"},
        RenderComponent{3},
        RenderTag{},
        SaveTag{},
        GameLayerTag{}
    );

    entity_manager.add_entity(
        NameComponent{"hud"},
        TextComponent{"", 32, sdl::SDL::Color::Orange},
        TransformComponent{{{0.5f * context.virtual_window_width, 0.1f * context.virtual_window_height}},
            {},
            Coordinate::Type::Screen
        },
        SpriteComponent{},
        RenderComponent{100},
        RenderTag{},
        HudTag{},
        SaveTag{},
        GameLayerTag{}
    );
}

}  // namespace survivor
