#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct RangeAttackSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core

