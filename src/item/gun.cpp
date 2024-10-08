#include <game/item/gun.hpp>

#include <wheel/utils.hpp>

#include <game/global.hpp>
#include <game/entity_manager.hpp>
#include <game/audio_manager.hpp>
#include <game/texture_manager.hpp>

#include <game/component/position.hpp>
#include <game/component/direction.hpp>
#include <game/component/inventory.hpp>
#include <game/component/actions.hpp>
#include <game/component/perk.hpp>
#include <game/component/aim_direction.hpp>

namespace wheel {

Gun::Gun(Data* data, Entity entity) : Weapon(data, entity) {
    int shoot_interval = data->shoot_interval;
    int reload_interval = data->reload_interval;
    action_map_["shoot"] = std::make_shared<MultiShotAction>(
        "shoot",
        SDLK_MOUSE_LEFT,
        [shoot_interval, &entity = entity_]() {
            float cooldown = shoot_interval;
            if (ecs.has_components<PerkComponent>(entity)) {
                auto& perk = ecs.get_component<PerkComponent>(entity);
                cooldown = cooldown * 100 / perk.shoot_speed_ratio;
            }
            return static_cast<int>(cooldown);
        },
        [this]() {
            shoot();
        }
    );

    action_map_["reload"] = std::make_shared<OneShotAction>(
        "reload",
        'r',
        [reload_interval, &entity = entity_]() {
            float cooldown = reload_interval;
            if (ecs.has_components<PerkComponent>(entity)) {
                auto& perk = ecs.get_component<PerkComponent>(entity);
                cooldown = cooldown * 100 / perk.reload_speed_ratio;
            }
            return static_cast<int>(cooldown);
        },
        [this]() {
            reload();
        }
    );

    ammo_ = data->clip;
}

// use AimDirectionComponent first
// if not, use DirectionComponent
void Gun::shoot() {
    if (reloading_) {
        return;
    }
    if (ammo_ <= 0) {
        reload();
        return;
    }
    ammo_ -= 1;

    auto& pos = ecs.get_component<PositionComponent>(entity_).vec;
    auto& direction = ecs.has_components<AimDirectionComponent>(entity_) ?
        ecs.get_component<AimDirectionComponent>(entity_).vec : ecs.get_component<DirectionComponent>(entity_).vec;
    int a = data().angle;
    int atk = data().atk;
    if (ecs.has_components<PerkComponent>(entity_)) {
        auto& perk = ecs.get_component<PerkComponent>(entity_);
        a = (float)a * 100 / perk.accuracy_ratio;
        atk = std::ceil((float)atk * perk.atk_ratio / 100);
    }
    for (int i = 0; i < data().count; ++i) {
        float angle = random.uniform<float>(-a, a) * M_PI / 180;
        auto dir = Vector2D<float>{direction.x * std::cos(angle) - direction.y * std::sin(angle),
                          direction.x * std::sin(angle) + direction.y * std::cos(angle)};
        EntityManager::instance().create_bullet(
            "bullet_shell",
            pos,
            dir,
            atk,
            data().penetration,
            entity_
        );
    }

    AudioManager::instance().play(data().name + "_shoot");
    // camera.shake(10, 3, 1);
}

void Gun::reload() {
    if (reloading_ || ammo_ == data().clip) {
        return;
    }

    reloading_ = true;
    timer_resource.add(data().reload_interval, 1, [&]() {
        reloading_ = false;
        ammo_ = data().clip;
    });

    AudioManager::instance().play(data().name + "_reload", data().reload_interval);
}

SDL_Texture* Gun::get_cursor_texture() const {
    return TextureManager::instance().get_texture("cross_cursor");
}

}  // namespace wheel
