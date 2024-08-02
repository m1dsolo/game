#include <game/item/floor.hpp>

#include <game/global.hpp>
#include <game/map.hpp>

#include <game/component/position.hpp>

namespace wheel {

bool Floor::use() {
    auto& position = ecs.get_component<PositionComponent>(entity_);
    return Map::instance().plant(data().name, entity_, position.vec);
}

}  // namespace wheel
