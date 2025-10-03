#include <core/system/track.hpp>
#include <core/global.hpp>
#include <core/component/transform.hpp>
#include <core/component/direction.hpp>
#include <core/component/track.hpp>

namespace core {

void TrackSystem::operator()() {
    for (auto [entity, transform, direction, track]
        :ecs.get_entity_and_components<TransformComponent, DirectionComponent, TrackComponent>()) {
        auto& pos0 = transform.global.position;
        if (ecs.has_entity(track.target)) {
            auto& pos1 = ecs.get_component<TransformComponent>(track.target).global.position;
            direction.move = direction.look = (pos1 - pos0).normalize();
        } else {
            ecs.remove_component<TrackComponent>(entity);
        }
    }
}

}  // namespace core
