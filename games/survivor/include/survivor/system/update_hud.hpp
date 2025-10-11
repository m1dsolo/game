#pragma once

namespace wheel {
class ECS;
}

namespace survivor {

struct UpdateHudSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace survivor
