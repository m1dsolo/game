#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct RenderSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core
