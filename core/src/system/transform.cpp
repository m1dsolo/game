#include <core/system/transform.hpp>
#include <core/util/hierarchy.hpp>
#include <core/component/transform.hpp>
#include <core/component/children.hpp>

#include <ecs/ecs.hpp>

namespace core {

void update_transform_(wheel::ECS& ecs);
void update_transform_(wheel::ECS& ecs, wheel::Entity entity, const TransformComponent& parent_transform);

void TransformSystem::operator()(wheel::ECS& ecs) {
    update_transform_(ecs);
}

void update_transform_(wheel::ECS& ecs) {
    auto root = Hierarchy::root();
    auto& transform = ecs.get_component<TransformComponent>(root);
    for (auto entity : Hierarchy::entities()) {
        update_transform_(ecs, entity, transform);
    }
}

void update_transform_(wheel::ECS& ecs, wheel::Entity entity, const TransformComponent& parent_transform) {
    if (!ecs.has_component<TransformComponent>(entity)) {
        return;
    }
    auto& transform = ecs.get_component<TransformComponent>(entity);

    transform.global.position = transform.local.position + parent_transform.global.position;
    transform.global.size = transform.local.size * parent_transform.global.scale;
    transform.global.scale = transform.local.scale * parent_transform.global.scale;
    transform.global.angle = transform.local.angle + parent_transform.global.angle;
    // transform.center = {
    //     parent_transform.center.x - transform.local_position.x,
    //     parent_transform.center.y - transform.local_position.y
    // };

    if (ecs.has_component<ChildrenComponent>(entity)) {
        auto& children = ecs.get_component<ChildrenComponent>(entity);
        for (auto child : children.entities) {
            update_transform_(ecs, child, transform);
        }
    }
}

}  // namespace core
