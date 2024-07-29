#include <game/item/potion.hpp>

namespace wheel {

void Potion::use() {
    for (auto& buff : data().buffs) {
        buff->on_effect(entity_);
    }
}

}  // namespace wheel
