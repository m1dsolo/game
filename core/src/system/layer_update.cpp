#include <core/system/layer_update.hpp>
#include <core/manager/layer.hpp>
#include <core/entity_event/remove_layer.hpp>

#include <ecs/ecs.hpp>

namespace core {

void LayerUpdateSystem::operator()(wheel::ECS& ecs) {
    LayerManager::instance().update();

    for (auto _ : ecs.get_events<RemoveLayerEvent>()) {
        LayerManager::instance().pop();
    }
}

}  // namespace core
