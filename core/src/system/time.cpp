#include <core/system/time.hpp>
#include <core/global.hpp>
#include <core/manager/time.hpp>

namespace core {

void TimeSystem::operator()() {
    auto& time_manager = TimeManager::instance();
    auto& timer = time_manager.timer();

    while (!timer.time_util_next()) {
        timer.update();
    }

    time_manager.update();

    int fps = config.fps;
    time_t target = timer.TIME_UNIT_PER_SECOND / fps;
    timer.sleep(std::max(static_cast<wheel::time_t>(0), target - time_manager.dt()));
}

}  // namespace core
