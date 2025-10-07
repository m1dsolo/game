#include <core/manager/collider.hpp>
#include <core/global.hpp>
#include <core/component/transform.hpp>
#include <core/component/parent.hpp>
#include <core/component/collider.hpp>
#include <core/tag/rigidbody.hpp>
#include <core/resource/context.hpp>
#include <core/util/hierarchy.hpp>

namespace core {

ColliderManager::ColliderManager() {
    const auto& context = ecs.get_resource<ContextResource>();
    init(context.map_width + 100.f, context.map_height + 100.f);
}

void ColliderManager::init(float w, float h) {
    quadtree_.clear();

    auto rect = wheel::Rect<float>{-w / 2, -h / 2, w / 2, h / 2};
    quadtree_.set_rect(rect);

    auto get_rect = [](wheel::Entity entity) -> wheel::Rect<float> {
        return {
            ecs.get_component<TransformComponent>(entity).global.position,
            ecs.get_component<ColliderComponent>(entity).size()
        };
    };
    quadtree_.set_get_rect(get_rect);
}

void ColliderManager::add(wheel::Entity entity) {
    quadtree_.add(entity);
}

void ColliderManager::remove(wheel::Entity entity) {
    quadtree_.remove(entity);
}

void ColliderManager::update() {
    quadtree_.update();
}

bool ColliderManager::is_colliding(wheel::Entity entity) {
    if (!ecs.has_components<ColliderComponent, RigidbodyTag>(entity)) {
        return false;
    }
    return !query<RigidbodyTag>(entity).empty();
}

std::vector<std::pair<wheel::Entity, wheel::Entity>> ColliderManager::query_all() const {
    return quadtree_.query_all() |
        std::views::filter([&](std::pair<wheel::Entity, wheel::Entity> p) {
            const auto& collider0 = ecs.get_component<ColliderComponent>(p.first);
            const auto& collider1 = ecs.get_component<ColliderComponent>(p.second);
            return collider0.is_overlapping(collider1);
        }) |
        std::ranges::to<std::vector<std::pair<wheel::Entity, wheel::Entity>>>();
}

std::vector<wheel::Entity> ColliderManager::query_(wheel::Entity entity) const {
    auto entities = quadtree_.query(entity);

    const auto& collider = ecs.get_component<ColliderComponent>(entity);
    return entities | 
        std::views::filter([&](wheel::Entity target) {
            return collider.is_overlapping(ecs.get_component<ColliderComponent>(target));
        }) |
        std::ranges::to<std::vector<wheel::Entity>>();
}

}  // namespace core
