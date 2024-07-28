#include <game/item/potion.hpp>

#include <game/audio_manager.hpp>

namespace wheel {

Potion::Potion(Data* data, Entity entity, Slot& slot) : Consumable(data, entity, slot) {
    data->action_map["use_potion"] = std::make_shared<OneShotAction>(
        "use_potion",
        ' ',
        []() {
            return 90;
        },
        [this]() {
            if (!uses_) {
                return;
            }
            on_effect();
            if (!--uses_) {
                this->slot().reduce(1);
                uses_ = this->data().max_uses;
            }

            AudioManager::instance().play("drink_potion");
        }
    );
}

void Potion::on_effect() {
    for (auto& buff : data().buffs) {
        buff->on_effect(entity_);
    }
}

}  // namespace wheel
