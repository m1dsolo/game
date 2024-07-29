#include <game/item/consumable.hpp>

#include <game/audio_manager.hpp>

namespace wheel {

Consumable::Consumable(Data* data, Entity entity, Slot& slot, const std::string& action_name, std::function<int()> get_cooldown, SDL_Keycode keycode)
    : Item(data, entity), slot_(slot), uses_(data->max_uses) {
    data->action_map[action_name] = std::make_shared<OneShotAction>(
        action_name,
        keycode,
        get_cooldown,
        [this, action_name]() {
            if (!uses_) {
                return;
            }
            use();
            if (!--uses_) {
                this->slot().reduce(1);
                uses_ = this->data().max_uses;
            }

            AudioManager::instance().play(action_name);
        }
    );
}

}  // namespace wheel
