#include <game/item/tower.hpp>

#include <game/global.hpp>
#include <game/entity_manager.hpp>

#include <game/resource/map.hpp>

#include <game/component/self.hpp>
#include <game/component/position.hpp>

namespace wheel {

void Tower::use() {
    auto entity = ecs.get_entities<SelfComponent>()[0];
    auto& position = ecs.get_component<PositionComponent>(entity);
    map_resource.plant(data().name, position.vec);
}

}  // namespace wheel
