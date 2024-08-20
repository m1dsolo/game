#include <game/system/ai.hpp>

#include <game/global.hpp>

#include <game/component/ai.hpp>

namespace wheel {

void AISystem::execute_impl() {
    for (auto [entity, ai] : ecs.get_entity_and_components<AIComponent>()) {
        ai.behavior_tree->update(entity);
    }
}

}  // namespace wheel
