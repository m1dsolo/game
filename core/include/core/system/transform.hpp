#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct TransformSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core
