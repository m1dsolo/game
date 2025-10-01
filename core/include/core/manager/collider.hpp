#pragma once

#include <core/global.hpp>

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

    std::vector<std::pair<wheel::Entity, wheel::Entity>> query_all() const;

    template <typename... ComponentTypes>
    std::vector<wheel::Entity> query(wheel::Entity entity) const {
        return query_(entity) |
            std::views::filter([&](wheel::Entity target) {
                return ecs.has_components<ComponentTypes...>(target);
            }) |
            std::ranges::to<std::vector<wheel::Entity>>();
    }

private:
    ColliderManager();
    ColliderManager(const ColliderManager&) = delete;

    std::vector<wheel::Entity> query_(wheel::Entity entity) const;

    wheel::QuadTree<wheel::Entity, std::function<wheel::Rect<float>(wheel::Entity)>> quadtree_;
};

}  // namespace core
