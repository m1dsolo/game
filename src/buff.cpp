#include <game/buff.hpp>

#include <game/component/buff.hpp>

namespace wheel {

void HPBuff::on_effect(Entity entity) {
    ecs.add_components(entity, HealBuffComponent{hp_});
}

}  // namespace wheel
