#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct TrackSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core
