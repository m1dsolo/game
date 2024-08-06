#include <game/card.hpp>

#include <game/global.hpp>

#include <game/component/hp.hpp>
#include <game/component/self.hpp>

namespace wheel {

void Card::use() {
    Entity entity = ecs.get_entities<SelfComponent>()[0];
    for (auto buff : buffs_) {
        buff(entity);
    }
}

}  // namespace wheel
