#include <core/util/hierarchy.hpp>
#include <core/global.hpp>
#include <core/component/children.hpp>
#include <core/component/parent.hpp>
#include <core/tag/root.hpp>

namespace core {

wheel::Entity Hierarchy::root() {
    return ecs.get_entity<RootTag>();
}

std::vector<wheel::Entity> Hierarchy::entities() {
    auto root_entity = root();
    return ecs.get_component<ChildrenComponent>(root_entity).entities;
}

wheel::Entity Hierarchy::detach_entity_from_parent(wheel::Entity entity) {
    auto parent = ecs.get_component<ParentComponent>(entity).entity;
    if (parent) {
        auto& children = ecs.get_component<ChildrenComponent>(parent).entities;
        std::erase(children, entity);
        if (children.empty()) {
            ecs.remove_component<ChildrenComponent>(parent);
        }
    }
    return parent;
}

void Hierarchy::attach_entity_to_parent(wheel::Entity entity, wheel::Entity parent) {
    if (!ecs.has_component<ChildrenComponent>(parent)) {
        ecs.add_component(parent, ChildrenComponent{});
    }
    ecs.get_component<ChildrenComponent>(parent).entities.emplace_back(entity);

    if (ecs.has_component<ParentComponent>(entity)) {
        detach_entity_from_parent(entity);
        ecs.get_component<ParentComponent>(entity).entity = parent;
    } else {
        ecs.add_component(entity, ParentComponent{parent});
    }
}

}  // namespace core
