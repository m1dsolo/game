#include <core/system/death.hpp>
#include <core/global.hpp>
#include <core/event/death.hpp>
#include <core/event/del_entity.hpp>

namespace core {

void DeathSystem::operator()() {
    for (auto [source, target] : ecs.get_events<DeathEvent>()) {
        if (ecs.has_entity(target)) {
            ecs.emplace_event<DelEntityEvent>(target);
        }
    }
}

}  // namespace core
