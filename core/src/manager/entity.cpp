#include <core/manager/entity.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/children.hpp>
#include <core/tag/root.hpp>
#include <core/tag/camera.hpp>

namespace core {

EntityManager::EntityManager() {
    ecs.add_entity(
        NameComponent{"root"},
        TransformComponent{},
        ChildrenComponent{},
        RootTag{}
    );

    add_entity(
        NameComponent{"camera"},
        TransformComponent{{0.f, 0.f}, {1920.f, 1080.f}},
        CameraTag{}
    );
}

void EntityManager::del_entity(wheel::Entity entity) {
    if (ecs.has_component<ChildrenComponent>(entity)) {
        for (auto child : ecs.get_component<ChildrenComponent>(entity).children) {
            del_entity(child);
        }
    }
    Hierarchy::detach_entity_from_parent(entity);
    ColliderManager::instance().remove(entity);
    ecs.del_entity(entity);
}

}  // namespace core
