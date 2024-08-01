#include <game/item/potion.hpp>

namespace wheel {

bool Potion::use() {
    for (auto& buff : data().buffs) {
        buff->on_effect(entity_);
    }
    return true;
}

}  // namespace wheel
