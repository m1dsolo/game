#include <core/utils/hierarchy.hpp>
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
    return ecs.get_component<ChildrenComponent>(root_entity).children;
}

wheel::Entity Hierarchy::detach_entity_from_parent(wheel::Entity entity) {
    auto parent = ecs.get_component<ParentComponent>(entity).parent;
    if (parent) {
        auto& children = ecs.get_component<ChildrenComponent>(parent).children;
        std::erase(children, entity);
        if (children.empty()) {
            ecs.del_component<ChildrenComponent>(parent);
        }
    }
    return parent;
}

std::unordered_set<wheel::Entity> Hierarchy::get_all_parents(wheel::Entity entity) {
    auto parents = std::unordered_set<wheel::Entity>{};
    while (ecs.has_component<ParentComponent>(entity)) {
        entity = ecs.get_component<ParentComponent>(entity).parent;
        parents.emplace(entity);
    }
    return parents;
}

}  // namespace core
