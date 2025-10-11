#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct LevelSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core
