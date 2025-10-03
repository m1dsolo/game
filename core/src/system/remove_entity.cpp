#include <core/system/remove_entity.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/event/remove_entity.hpp>

namespace core {

void RemoveEntitySystem::operator()() {
    for (auto [entity] : ecs.get_events<RemoveEntityEvent>()) {
        if (ecs.has_entity(entity)) {
            EntityManager::instance().remove_entity(entity);
        }
    }
}

}  // namespace core
