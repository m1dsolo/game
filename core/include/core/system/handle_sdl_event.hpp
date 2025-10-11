#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct HandleSDLEventSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core
