#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct GenerateHPFloatTextSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core
