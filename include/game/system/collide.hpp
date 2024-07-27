#pragma once

#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>

namespace wheel {

class CollideSystem : public Singleton<CollideSystem> {
    friend class Singleton<CollideSystem>;

public:
    void execute();

private:
    CollideSystem() = default;
    ~CollideSystem() = default;
    CollideSystem(const CollideSystem&) = delete;

    void collide();
    void bullet_out_of_boundary();
    void auto_pickup();
};

}  // namespace wheel
