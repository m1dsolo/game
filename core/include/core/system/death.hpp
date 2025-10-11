#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct DeathSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core
