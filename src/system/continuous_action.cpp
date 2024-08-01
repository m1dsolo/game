#include <game/system/continuous_action.hpp>

#include <game/global.hpp>

#include <game/component/continuous_action.hpp>

namespace wheel {

void ContinuousActionSystem::execute_impl() {
    for (auto [entity, continuous_action] : ecs.get_entity_and_components<ContinuousActionComponent>()) {
        if (continuous_action.action()) {
            ecs.del_components<ContinuousActionComponent>(entity);
        }
    }
}

}  // namespace wheel
