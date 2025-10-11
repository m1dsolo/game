#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct AnimationSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core
