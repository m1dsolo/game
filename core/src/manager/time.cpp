#include <core/manager/time.hpp>

namespace core {

void TimeManager::update() {
    current_frame_time_ = timer_.tick();
    dt_ = current_frame_time_ - last_frame_time_;
    last_frame_time_ = current_frame_time_;
}

}  // namespace core
