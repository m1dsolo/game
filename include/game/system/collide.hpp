#pragma once

#include <game/system/base.hpp>
#include <game/map.hpp>

#include <wheel/ecs.hpp>
#include <wheel/geometry.hpp>
#include <wheel/quadtree.hpp>

namespace wheel {

class CollideSystem : public BaseSystem<CollideSystem> {
    friend class BaseSystem<CollideSystem>;

public:
    void execute_impl() override;

private:
    CollideSystem();
    ~CollideSystem() = default;
    CollideSystem(const CollideSystem&) = delete;

    void update_collision_manager();
    void collide();
    void bullet_out_of_boundary();
    void bullet_collide_structure();
    void pick_range();

    void collide_damage(Entity entity0, Entity entity1);
    void pick(Entity entity0, Entity entity1);

};

}  // namespace wheel
