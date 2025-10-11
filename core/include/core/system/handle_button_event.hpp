#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct HandleButtonEventSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core
