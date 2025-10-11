#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct PickItemSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core

