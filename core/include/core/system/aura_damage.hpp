#pragma once

#include <ecs/entity.hpp>
#include <wheel/timer.hpp>
#include <wheel/utils.hpp>

#include <unordered_map>

namespace wheel {
class ECS;
}

namespace core {

struct AuraDamageSystem {
    void operator()(wheel::ECS& ecs);

private:
    std::unordered_map<std::pair<wheel::Entity, wheel::Entity>, wheel::timer_id_t, wheel::Utils::hash_pair_32<wheel::Entity, wheel::Entity>> timer_id_mp_;
};

}  // namespace core
