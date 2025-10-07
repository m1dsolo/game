#include <core/manager/render.hpp>
#include <core/component/render.hpp>

namespace core {

void RenderManager::add(wheel::Entity entity) {
    auto layer = ecs.get_component<RenderComponent>(entity).layer;
    if (layer < 0) {
        return;
    }
    if (layer >= static_cast<int>(layer_entities_.size())) {
        layer_entities_.resize(layer + 1);
    }
    layer_entities_[layer].add(entity);
}

void RenderManager::remove(wheel::Entity entity) {
    auto layer = ecs.get_component<RenderComponent>(entity).layer;
    if (layer < 0 || layer >= static_cast<int>(layer_entities_.size())) {
        return;
    }
    layer_entities_[layer].remove(entity);
}

}  // namespace core
