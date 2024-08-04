#include <game/action.hpp>

#include <game/audio_manager.hpp>
#include <game/game_manager.hpp>

#include <game/component/move.hpp>
#include <game/component/position.hpp>
#include <game/component/direction.hpp>
#include <game/component/invincible.hpp>
#include <game/component/perk.hpp>
#include <game/component/inventory.hpp>

namespace wheel {

void OneShotAction::start_impl() {
    if (ready_ && start_func_) {
        if (get_cooldown_) {
            int cooldown = get_cooldown_();
            if (cooldown) {
                ready_ = false;
                timer_resource.add(get_cooldown_(), 1, [&]() {
                    ready_ = true;
                });
            }
        }
        start_ = true;
        start_func_();
    }
}

void OneShotAction::finish() {
    if (start_ && finish_func_) {
        start_ = false;
        finish_func_();
    }
}

void MultiShotAction::start_impl() {
    if (!start_ && ready_ && func_) {
        func_();
        ready_ = false;
        start_ = true;
        if (get_cooldown_) {
            int cooldown = get_cooldown_();
            if (cooldown) {
                timer_func_ = [&, cooldown]() {
                    if (start_) {
                        func_();
                        timer_resource.add(cooldown, 1, timer_func_);
                    } else {
                        ready_ = true;
                    }
                };
                timer_resource.add(cooldown, 1, timer_func_);
            }
        }
    }
}

void MultiShotAction::finish() {
    start_ = false;
}

void ClickAction::start_impl(float x, float y) {
    if (ready_ && start_func_) {
        if (get_cooldown_) {
            int cooldown = get_cooldown_();
            if (cooldown) {
                ready_ = false;
                timer_resource.add(get_cooldown_(), 1, [&]() {
                    ready_ = true;
                });
            }
        }
        start_ = true;
        start_func_(x, y);
    }
}

void ClickAction::finish() {
    if (start_ && finish_func_) {
        start_ = false;
        finish_func_();
    }
}

QuitAction::QuitAction() : OneShotAction("quit", 'q') {
    start_func_ = []() {
        GameManager::instance().quit();
    };
}

MoveUpAction::MoveUpAction(Entity entity) : OneShotAction("move_up", 'w') {
    start_func_ = [entity]() {
        auto& move = ecs.get_component<MoveComponent>(entity);
        move.up = true;
    };
    finish_func_ = [entity]() {
        auto& move = ecs.get_component<MoveComponent>(entity);
        move.up = false;
    };
}

MoveDownAction::MoveDownAction(Entity entity) : OneShotAction("move_down", 's') {
    start_func_ = [entity]() {
        auto& move = ecs.get_component<MoveComponent>(entity);
        move.down = true;
    };
    finish_func_ = [entity]() {
        auto& move = ecs.get_component<MoveComponent>(entity);
        move.down = false;
    };
}

MoveLeftAction::MoveLeftAction(Entity entity) : OneShotAction("move_left", 'a') {
    start_func_ = [entity]() {
        auto& move = ecs.get_component<MoveComponent>(entity);
        move.left = true;
    };
    finish_func_ = [entity]() {
        auto& move = ecs.get_component<MoveComponent>(entity);
        move.left = false;
    };
}

MoveRightAction::MoveRightAction(Entity entity) : OneShotAction("move_right", 'd') {
    start_func_ = [entity]() {
        auto& move = ecs.get_component<MoveComponent>(entity);
        move.right = true;
    };
    finish_func_ = [entity]() {
        auto& move = ecs.get_component<MoveComponent>(entity);
        move.right = false;
    };
}

FlashAction::FlashAction(Entity entity) : OneShotAction("flash", 'f') {
    get_cooldown_ = [&, entity]() {
        double cooldown = 300;
        // if (ecs.has_components<PerkComponent>(entity)) {
        //     auto& perk = ecs.get_component<PerkComponent>(entity);
        // }
        return static_cast<int>(cooldown);
    };

    start_func_ = [&, entity]() {
        auto& position = ecs.get_component<PositionComponent>(entity);
        auto& direction = ecs.get_component<DirectionComponent>(entity).vec;

        position.vec += direction.normalize() * 100;

        // add invincible
        if (ecs.has_components<InvincibleComponent>(entity)) {
            auto& invincible = ecs.get_component<InvincibleComponent>(entity);
            invincible.layer++;
        } else {
            ecs.add_components<InvincibleComponent>(entity, {1});
        }
        timer_resource.add(10, 1, [&]() {
            if (ecs.has_components<InvincibleComponent>(entity)) {
                auto& invincible = ecs.get_component<InvincibleComponent>(entity);
                if (!--invincible.layer) {
                    ecs.del_components<InvincibleComponent>(entity);
                }
            }
        });

        AudioManager::instance().play("flash");
    };
}

SwitchSelectedItemAction::SwitchSelectedItemAction(Entity entity, int idx)
    : OneShotAction(std::format("switch_item_{}", idx), '0' + (idx % 10)) {
    start_func_ = [entity, idx]() {
        auto& inventory = ecs.get_component<InventoryComponent>(entity).inventory;
        inventory.select(idx);
    };
}

}  // namespace wheel
