#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct HPChangeSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core
