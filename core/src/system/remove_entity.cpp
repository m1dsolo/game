#include <core/system/remove_entity.hpp>
#include <core/manager/entity.hpp>
#include <core/entity_event/remove_entity.hpp>

#include <ecs/ecs.hpp>

namespace core {

void RemoveEntitySystem::operator()(wheel::ECS& ecs) {
    auto entities = ecs.get_entities<RemoveEntityEvent>() | std::ranges::to<std::vector>();
    for (auto entity : entities) {
        EntityManager::instance().remove_entity(entity);
    }
}

}  // namespace core
