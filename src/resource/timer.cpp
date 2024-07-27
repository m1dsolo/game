#include <game/resource/timer.hpp>

#include <game/global.hpp>

namespace wheel {

TimerResource::TimerResource() {
    last = timer.tick();
    auto& config_resource = ecs.get_resource<ConfigResource>();
    us_per_frame = timer.UNITS_IN_SECOND / config_resource.fps;
}

TimerResource::~TimerResource() {
}

}  // namespace wheel
