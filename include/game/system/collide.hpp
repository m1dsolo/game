#pragma once

#include <game/system/base.hpp>

namespace wheel {

class CollideSystem : public BaseSystem<CollideSystem> {
    friend class BaseSystem<CollideSystem>;

public:
    void execute_impl() override;

private:
    CollideSystem() : BaseSystem(false) {}
    ~CollideSystem() = default;
    CollideSystem(const CollideSystem&) = delete;

    void collide();
    void bullet_out_of_boundary();
    void bullet_collide_structure();
    void trap_collide_enemy();
    void pick_range();
    void auto_pickup();
};

}  // namespace wheel
