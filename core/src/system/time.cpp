#include <core/system/time.hpp>
#include <core/manager/time.hpp>
#include <core/resource/context.hpp>

#include <ecs/ecs.hpp>

namespace core {

void TimeSystem::operator()(wheel::ECS& ecs) {
    auto& time_manager = TimeManager::instance();
    auto& timer = time_manager.timer();

    while (!timer.time_util_next()) {
        timer.update();
    }

    time_manager.update();

    int fps = ecs.get_resource<ContextResource>().fps;
    time_t target = timer.TIME_UNIT_PER_SECOND / fps;
    timer.sleep(std::max(static_cast<wheel::time_t>(0), target - time_manager.dt()));
}

}  // namespace core
