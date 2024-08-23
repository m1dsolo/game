#pragma once

#include <functional>
#include <vector>

#include <wheel/singleton.hpp>
#include <wheel/quadtree.hpp>
#include <wheel/ecs.hpp>

namespace wheel {

class CollisionManager : public Singleton<CollisionManager> {
    friend class Singleton<CollisionManager>;

public:
    void add(Entity entity, bool dynamic);
    void remove(Entity entity);
    void update();

    std::vector<std::pair<Entity, Entity>> find_all_dynamic_intersections();
    bool is_collision_with_static(Entity entity);

private:
    CollisionManager();
    ~CollisionManager() = default;
    CollisionManager(const CollisionManager&) = delete;

    QuadTree<Entity, std::function<Rect<float>(Entity)>> static_quadtree_;
    QuadTree<Entity, std::function<Rect<float>(Entity)>> dynamic_quadtree_;
};

}  // namespace wheel 
