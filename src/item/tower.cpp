#include <game/item/tower.hpp>

#include <game/global.hpp>
#include <game/map.hpp>
#include <game/texture_manager.hpp>

#include <game/component/position.hpp>

namespace wheel {

bool Tower::use() {
    auto& position = ecs.get_component<PositionComponent>(entity_);
    return Map::instance().plant(data().name, entity_, position.vec);
}

}  // namespace wheel
