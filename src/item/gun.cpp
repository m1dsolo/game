#include <game/item/gun.hpp>

#include <wheel/utils.hpp>

#include <game/global.hpp>
#include <game/entity_manager.hpp>
#include <game/audio_manager.hpp>

#include <game/component/position.hpp>
#include <game/component/direction.hpp>
#include <game/component/inventory.hpp>
#include <game/component/actions.hpp>
#include <game/component/perk.hpp>

namespace wheel {

Gun::Gun(Data* data, Entity entity) : Weapon(data, entity) {
    int shoot_interval = data->shoot_interval;
    int reload_interval = data->reload_interval;
    data->action_map["shoot"] = std::make_shared<MultiShotAction>(
        "shoot",
        ' ',
        [shoot_interval, &entity = entity_]() {
            double cooldown = shoot_interval;
            if (ecs.has_components<PerkComponent>(entity)) {
                auto& perk = ecs.get_component<PerkComponent>(entity);
                cooldown /= perk.shoot_speed_ratio;
            }
            return static_cast<int>(cooldown);
        },
        [this]() {
            shoot();
        }
    );

    data->action_map["reload"] = std::make_shared<OneShotAction>(
        "reload",
        'r',
        [reload_interval, &entity = entity_]() {
            double cooldown = reload_interval;
            if (ecs.has_components<PerkComponent>(entity)) {
                auto& perk = ecs.get_component<PerkComponent>(entity);
                cooldown /= perk.reload_speed_ratio;
            }
            return static_cast<int>(cooldown);
        },
        [this]() {
            reload();
        }
    );

    ammo_ = data->clip;
}

void Gun::shoot() {
    if (reloading_) {
        return;
    }
    if (ammo_ == 0) {
        data().action_map["reload"]->start();
        return;
    }
    ammo_ -= 1;

    for (int i = 0; i < data().count; ++i) {
        auto position = ecs.get_component<PositionComponent>(entity_);
        auto direction = ecs.get_component<DirectionComponent>(entity_);
        int a = data().angle;
        if (ecs.has_components<PerkComponent>(entity_)) {
            auto& perk = ecs.get_component<PerkComponent>(entity_);
            a = (double)a / perk.accuracy_ratio;
        }

        double angle = random.uniform(-a, a) * M_PI / 180;
        direction.vec = {direction.vec.x * std::cos(angle) - direction.vec.y * std::sin(angle),
                          direction.vec.x * std::sin(angle) + direction.vec.y * std::cos(angle)};
        EntityManager::instance().create_bullet(
            "bullet_shell",
            position.vec,
            direction.vec,
            data().atk,
            data().penetration,
            entity_
        );
    }

    AudioManager::instance().play(data().name + "_shoot");
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

}  // namespace wheel
