#include <core/system/time.hpp>
#include <core/global.hpp>
#include <core/resource/context.hpp>
#include <core/resource/time.hpp>

namespace core {

void TimeSystem::operator()(wheel::ECS& ecs) {
    auto time = timer.tick();
    auto dt = time - last_frame_time_;
    last_frame_time_ = time;

    ecs.get_resource<TimeResource>().dt = dt;

    int fps = ecs.get_resource<ContextResource>().fps;
    time_t target = timer.TIME_UNIT_PER_SECOND / fps;
    timer.sleep(std::max(static_cast<wheel::time_t>(0), target - dt));
}

}  // namespace core
