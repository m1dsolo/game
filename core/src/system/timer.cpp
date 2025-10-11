#include <core/system/timer.hpp>
#include <core/global.hpp>

#include <ecs/ecs.hpp>

namespace core {

void TimerSystem::operator()(wheel::ECS& ecs) {
    while (!timer.time_util_next()) {
        timer.update();
    }
}

}  // namespace core
