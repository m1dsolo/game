#include <game/item/trap.hpp>

#include <game/global.hpp>
#include <game/map.hpp>

#include <game/component/position.hpp>

namespace wheel {

bool Trap::use() {
    auto& position = ecs.get_component<PositionComponent>(entity_);
    return Map::instance().plant(data().name, entity_, position.vec);
}

}  // namespace wheel
