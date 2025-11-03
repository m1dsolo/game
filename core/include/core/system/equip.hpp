#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct EquipSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace core
