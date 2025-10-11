#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct TimeSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core
