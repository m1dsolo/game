#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct MoveSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core
