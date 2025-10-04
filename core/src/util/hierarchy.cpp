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

std::unordered_set<wheel::Entity> Hierarchy::get_all_parents(wheel::Entity entity) {
    auto parents = std::unordered_set<wheel::Entity>{};
    while (ecs.has_component<ParentComponent>(entity)) {
        entity = ecs.get_component<ParentComponent>(entity).entity;
        parents.emplace(entity);
    }
    return parents;
}

std::unordered_set<wheel::Entity> Hierarchy::get_all_children(wheel::Entity entity) {
    auto children = std::unordered_set<wheel::Entity>{};
    if (ecs.has_component<ChildrenComponent>(entity)) {
        for (auto child : ecs.get_component<ChildrenComponent>(entity).entities) {
            children.emplace(child);
            auto grand_children = get_all_children(child);
            children.insert(grand_children.begin(), grand_children.end());
        }
    }
    return children;
}

std::unordered_set<wheel::Entity> Hierarchy::get_self_and_all_parents_children(wheel::Entity entity) {
    auto entities = get_all_parents(entity);
    entities.emplace(entity);
    for (auto parent : entities) {
        auto children = get_all_children(parent);
        entities.insert(children.begin(), children.end());
    }
    return entities;
}

}  // namespace core
