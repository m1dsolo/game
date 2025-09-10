#include <core/system/del_entity_event.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/event/del_entity.hpp>

namespace core {

void DelEntityEventSystem::update_impl() {
    for (auto [entity] : ecs.get_events<DelEntityEvent>()) {
        if (ecs.has_entity(entity)) {
            EntityManager::instance().del_entity(entity);
        }
    }
}

}  // namespace core
