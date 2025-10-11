#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct TimerSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core
