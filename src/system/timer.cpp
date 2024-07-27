#include <game/system/timer.hpp>

#include <game/global.hpp>

namespace wheel {

void TimerSystem::execute() {
    update();
}

void TimerSystem::update() {
    while (timer.time_to_sleep() == 0) {
        timer.update();
    }

    timer_t now = timer.tick();
    timer_t d = now - timer_resource.last;
    timer.sleep(std::max((timer_t)0, timer_resource.us_per_frame - d));
    timer_resource.delta = std::max(timer_resource.us_per_frame, d);
    timer_resource.last = now;
}

}  // namespace wheel
