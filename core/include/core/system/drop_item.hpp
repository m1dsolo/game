#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct DropItemSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core
