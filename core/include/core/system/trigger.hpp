#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct TriggerSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core
