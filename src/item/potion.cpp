#include <game/item/potion.hpp>

namespace wheel {

bool Potion::use() {
    for (auto& buff : data().buffs) {
        buff(entity_);
    }
    return true;
}

}  // namespace wheel
