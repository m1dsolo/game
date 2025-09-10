#include <core/manager/collider.hpp>
#include <core/global.hpp>
#include <core/component/transform.hpp>
#include <core/component/parent.hpp>
#include <core/component/collider.hpp>
#include <core/component/trigger.hpp>
#include <core/utils/hierarchy.hpp>

namespace core {

ColliderManager::ColliderManager() {
    init(config.map_width + 20.f, config.map_height + 20.f);
}

void ColliderManager::init(float w, float h) {
    static_quadtree_.clear();
    dynamic_quadtree_.clear();

    auto rect = wheel::Rect<float>{0.f, 0.f, w, h};
    static_quadtree_.set_rect(rect);
    dynamic_quadtree_.set_rect(rect);

    auto get_rect = [](wheel::Entity entity) -> wheel::Rect<float> {
        const auto& size = ecs.has_component<ColliderComponent>(entity) ?
            ecs.get_component<ColliderComponent>(entity).size :
            ecs.get_component<TriggerComponent>(entity).size;

        return {
            ecs.get_component<TransformComponent>(entity).global.position,
            size
        };
    };
    static_quadtree_.set_get_rect(get_rect);
    dynamic_quadtree_.set_get_rect(get_rect);
}

void ColliderManager::add(wheel::Entity entity) {
    if (is_dynamic_(entity)) {
        dynamic_quadtree_.add(entity);
    } else {
        static_quadtree_.add(entity);
    }
}

void ColliderManager::remove(wheel::Entity entity) {
    if (is_dynamic_(entity)) {
        dynamic_quadtree_.remove(entity);
    } else {
        static_quadtree_.remove(entity);
    }
}

void ColliderManager::update() {
    dynamic_quadtree_.update();
}

bool ColliderManager::is_colliding(wheel::Entity entity) {
    if (!ecs.has_component<ColliderComponent>(entity)) {
        return false;
    }
    return !query(entity).empty();
}

std::vector<wheel::Entity> ColliderManager::query(wheel::Entity entity) const {
    auto entities = dynamic_quadtree_.query(entity);
    auto static_entities = static_quadtree_.query(entity);
    entities.insert(entities.end(), static_entities.begin(), static_entities.end());

    auto parents = Hierarchy::get_all_parents(entity);
    return entities | 
        std::views::filter([&](wheel::Entity target) {
            return target != entity &&
                !parents.contains(target) &&
                ecs.has_component<ColliderComponent>(target);
        }) |
        std::ranges::to<std::vector<wheel::Entity>>();
}

bool ColliderManager::is_dynamic_(wheel::Entity entity) const {
    bool dynamic = false;
    if (ecs.has_component<ColliderComponent>(entity)) {
        dynamic = ecs.get_component<ColliderComponent>(entity).dynamic;
    } else if (ecs.has_component<TriggerComponent>(entity)) {
        dynamic = ecs.get_component<TriggerComponent>(entity).dynamic;
    }
    return dynamic;
}

}  // namespace core
