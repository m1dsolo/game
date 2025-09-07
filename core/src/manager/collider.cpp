#include <core/manager/collider.hpp>
#include <core/global.hpp>
#include <core/component/transform.hpp>
#include <core/component/collider.hpp>

namespace core {

ColliderManager::ColliderManager() {
    init(1920.f, 1080.f);
}

void ColliderManager::init(float w, float h) {
    static_quadtree_.clear();
    dynamic_quadtree_.clear();

    auto rect = wheel::Rect<float>{0.f, 0.f, w, h};
    static_quadtree_.set_rect(rect);
    dynamic_quadtree_.set_rect(rect);

    auto get_rect = [](wheel::Entity entity) -> wheel::Rect<float> {
        return {
            ecs.get_component<TransformComponent>(entity).global.position,
            ecs.get_component<ColliderComponent>(entity).size
        };
    };
    static_quadtree_.set_get_rect(get_rect);
    dynamic_quadtree_.set_get_rect(get_rect);
}

void ColliderManager::add(wheel::Entity entity) {
    if (!ecs.has_component<ColliderComponent>(entity)) {
        return;
    }
    const auto& collider = ecs.get_component<ColliderComponent>(entity);
    if (collider.dynamic) {
        dynamic_quadtree_.add(entity);
    } else {
        static_quadtree_.add(entity);
    }
}

void ColliderManager::remove(wheel::Entity entity) {
    if (!ecs.has_component<ColliderComponent>(entity)) {
        return;
    }
    auto& collider = ecs.get_component<ColliderComponent>(entity);
    if (collider.dynamic) {
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
    bool dynamic = ecs.get_component<ColliderComponent>(entity).dynamic;
    return (dynamic_quadtree_.query(entity).size() - dynamic) ||
           (static_quadtree_.query(entity).size() - !dynamic);
}

std::vector<wheel::Entity> ColliderManager::query(wheel::Entity entity) const {
    auto entities = dynamic_quadtree_.query(entity);
    auto static_entities = static_quadtree_.query(entity);
    entities.insert(entities.end(), static_entities.begin(), static_entities.end());
    return entities;
}

std::vector<wheel::Entity> ColliderManager::query(const wheel::Rect<float>& rect) const {
    auto entities = dynamic_quadtree_.query(rect);
    auto static_entities = static_quadtree_.query(rect);
    entities.insert(entities.end(), static_entities.begin(), static_entities.end());
    return entities;
}

}  // namespace core
