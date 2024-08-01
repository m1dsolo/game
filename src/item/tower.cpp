#include <game/item/tower.hpp>

#include <game/global.hpp>
#include <game/map.hpp>

#include <game/component/self.hpp>
#include <game/component/position.hpp>

namespace wheel {

void Tower::use() {
    auto entity = ecs.get_entities<SelfComponent>()[0];
    auto& position = ecs.get_component<PositionComponent>(entity);
    Map::instance().plant(data().name, position.vec);
}

}  // namespace wheel
