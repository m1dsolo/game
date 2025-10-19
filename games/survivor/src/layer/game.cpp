#include <survivor/layer/game.hpp>
#include <survivor/manager/wave.hpp>
#include <survivor/tag/hud.hpp>

#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/layer.hpp>
#include <core/manager/sprite.hpp>
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
#include <core/component/fraction.hpp>
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
#include <core/resource/context.hpp>

using namespace core;

namespace survivor {

void GameLayer::on_attach() {
    const auto& context = ecs.get_resource<ContextResource>();

    auto& entity_manager = EntityManager::instance();

    auto bunny = entity_manager.add_entity(
        NameComponent{"bunny"},
        TransformComponent{},
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
        HPComponent{100},
        LevelComponent{},
        FractionComponent{0},
        RigidbodyTag{},
        RenderTag{},
        InputTag{},
        PickupItemTag{}
    );

    auto inventory = ecs.get_entity<InventoryComponent>();
    auto& items = ecs.get_component<InventoryComponent>().items;
    items[0] = entity_manager.add_entity(
        inventory,
        NameComponent{"m4a1"},
        TransformComponent{},
        SpriteComponent{"m4a1"},
        RenderComponent{5},
        ItemComponent{"m4a1", 1}
    );
    items[1] = entity_manager.add_entity(
        inventory,
        NameComponent{"usp"},
        TransformComponent{},
        SpriteComponent{"usp"},
        RenderComponent{5},
        ItemComponent{"usp", 3}
    );

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
        MasterComponent{bunny}
    );

    auto camera = ecs.get_entity<CameraTag>();
    Hierarchy::attach_entity_to_parent(camera, bunny);

    SpriteManager::instance().set("pink_filled_circle", Sprite{
        sdl::SDL::create_filled_circle_texture(150.f, sdl::SDL::PINK),
        {0.f, 0.f, 300.f, 300.f}
    });
    auto damage_aura = entity_manager.add_entity(
        bunny,
        NameComponent{"damage_aura"},
        TransformComponent{},
        SpriteComponent{"pink_filled_circle"},
        RenderComponent{1},
        ColliderComponent{
            wheel::Circle<float>{150.f},
            ColliderLayer::Trigger,
            ColliderLayer::Enemy
        },
        AuraDamageComponent{8, 500000.f},
        MasterComponent{bunny},
        RenderTag{}
    );

    SpriteManager::instance().set("auto_shoot", Sprite{
        sdl::SDL::create_circle_texture(250.f, sdl::SDL::RED),
        {0.f, 0.f, 500.f, 500.f}
    });
    auto auto_shoot = entity_manager.add_entity(
        bunny,
        NameComponent("auto_shoot"),
        TransformComponent{},
        ColliderComponent{
            wheel::Circle<float>{250.f},
            ColliderLayer::Trigger,
            ColliderLayer::Enemy
        },
        RangeAttackComponent{
            .damage = 7,
            .interval = 200000,
            .projectile_speed = 500.f,
            .projectile_sprite_name = "bullet",
            .range_attack_sound_name = "m4a1/shoot.wav"
        },
        ReloadComponent{30, 2000000, "m4a1/reload.wav"},
        MasterComponent{bunny},
        SpriteComponent{"auto_shoot"},
        RenderComponent{1},
        RenderTag{}
    );

    auto fox = entity_manager.add_entity(
        NameComponent{"fox"},
        TransformComponent{{50.f, 0.f}},
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
        RenderComponent{1},
        HPComponent{100},
        LevelComponent{},
        FractionComponent{0},
        TrackComponent{bunny, 50.f},
        RigidbodyTag{},
        RenderTag{}
    );
    auto fox_auto_shoot = entity_manager.add_entity(
        fox,
        NameComponent("fox_auto_shoot"),
        TransformComponent{{0.f, 0.f}, {300.f, 300.f}},
        ColliderComponent{
            wheel::Circle<float>{150.f},
            ColliderLayer::Trigger,
            ColliderLayer::Enemy
        },
        RangeAttackComponent{
            .damage = 10,
            .interval = 500000,
            .projectile_speed = 500.f,
            .projectile_sprite_name = "bullet",
            .range_attack_sound_name = "usp/shoot.wav"
        },
        ReloadComponent{12, 1000000, "usp/reload.wav"},
        MasterComponent{bunny},
        SpriteComponent{"auto_shoot"},
        RenderComponent{1},
        RenderTag{}
    );

    // auto house = entity_manager.add_entity(
    //     NameComponent{"house"},
    //     TransformComponent{{100.f, 0.f}, {48.f, 48.f}},
    //     SpriteComponent{"house"},
    //     ColliderComponent{{48.f, 48.f}, false},
    //     RenderComponent{0}
    // );

    entity_manager.add_entity(
        TextComponent{"Hud", 32, sdl::SDL::ORANGE},
        TransformComponent{
            {0.5f * context.virtual_window_width, 0.1f * context.virtual_window_height},
            {0.f, 0.f},
            {1.f, 1.f},
            Coordinate::Type::Screen
        },
        SpriteComponent{},
        RenderComponent{4},
        RenderTag{},
        HudTag{}
    );

    core::GameLayer::on_attach();

    WaveManager::instance().generate_waves();
}

// TODO
void GameLayer::on_detach() {
}

}  // namespace survivor
