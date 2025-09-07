#pragma once

#include <ecs/entity.hpp>
#include <wheel/singleton.hpp>
#include <wheel/quadtree.hpp>

#include <functional>

namespace core {

class ColliderManager : public wheel::Singleton<ColliderManager> {
    friend wheel::Singleton<ColliderManager>;

public:
    void init(float w, float h);
    void add(wheel::Entity entity);
    void remove(wheel::Entity entity);
    void update();
    bool is_colliding(wheel::Entity entity);
    std::vector<wheel::Entity> query(wheel::Entity entity) const;
    std::vector<wheel::Entity> query(const wheel::Rect<float>& rect) const;

private:
    ColliderManager();
    ColliderManager(const ColliderManager&) = delete;

    wheel::QuadTree<wheel::Entity, std::function<wheel::Rect<float>(wheel::Entity)>> static_quadtree_;
    wheel::QuadTree<wheel::Entity, std::function<wheel::Rect<float>(wheel::Entity)>> dynamic_quadtree_;
};

}  // namespace core
