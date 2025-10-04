#include <core/system/remove_entity.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/entity_event/remove_entity.hpp>

namespace core {

void RemoveEntitySystem::operator()() {
    auto entities = ecs.get_entity_and_components<RemoveEntityEvent>() |
        std::views::transform([](auto&& tuple) {
            return std::get<0>(tuple);
        }) |
        std::ranges::to<std::vector<wheel::Entity>>();

    for (auto entity : entities) {
        EntityManager::instance().remove_entity(entity);
    }
}

}  // namespace core
