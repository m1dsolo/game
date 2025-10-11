#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct AttackSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core

