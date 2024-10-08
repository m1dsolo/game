#include <game/item/consumable.hpp>

#include <game/audio_manager.hpp>
#include <game/texture_manager.hpp>
#include <game/map.hpp>
#include <game/slot.hpp>

#include <game/component/position.hpp>
#include <game/component/direction.hpp>
#include <game/component/velocity.hpp>
#include <game/component/continuous_action.hpp>

namespace wheel {

Consumable::Consumable(Data* data, Entity entity, const std::string& action_name, std::function<int()> get_cooldown, SDL_Keycode keycode)
    : Item(data, entity), uses_(data->max_uses) {
    action_map_[action_name] = std::make_shared<OneShotAction>(
        action_name,
        keycode,
        get_cooldown,
        [this, action_name]() {
            if (!uses_) {
                return;
            }
            if (use()) {
                AudioManager::instance().play(action_name);
                reduce_uses();
            }
        }
    );

    // TODO: simplify logic
    std::string click_action_name = "move_and_" + action_name;
    action_map_[click_action_name] = std::make_shared<ClickAction>(
        click_action_name,
        SDLK_MOUSE_LEFT,
        [] { return 0; },
        [this, action_name](float x, float y) {
            auto& pos = ecs.get_component<PositionComponent>(entity_).vec;
            auto& dir = ecs.get_component<DirectionComponent>(entity_).vec;

            auto& map = Map::instance();
            auto map_pos = camera.screen2world({x, y});
            auto target_idx = map.pos2idx(map_pos);
            auto target = map.idx2pos(target_idx.first, target_idx.second);
            dir = (target - pos).normalize();

            auto& velocity = ecs.get_component<VelocityComponent>(entity_);
            velocity.speed = velocity.max_speed;
            ecs.add_components<ContinuousActionComponent>(entity_, {[this, target, action_name, &pos, &map]() {
                // arrived
                if (pos.distance(target) <= (float)map.TILE_SIZE / 4) {
                    if (!uses_) {
                        return true;
                    }
                    if (use()) {
                        AudioManager::instance().play(action_name);
                        reduce_uses();
                    }
                    return true;
                }
                return false;
            }});
        }
    );
}

SDL_Texture* Consumable::get_cursor_texture() const {
    return TextureManager::instance().get_texture(data().name);
}

void Consumable::reduce_uses() {
    if (!--uses_) {
        this->slot_->reduce(1);
        uses_ = this->data().max_uses;
    }
}

}  // namespace wheel
