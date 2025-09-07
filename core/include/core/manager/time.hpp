#pragma once

#include <wheel/singleton.hpp>
#include <wheel/timer.hpp>

namespace core {

class TimeManager : public wheel::Singleton<TimeManager> {
    friend wheel::Singleton<TimeManager>;

public:
    void update();

    wheel::time_t dt() const { return dt_; }

    wheel::Timer& timer() { return timer_; };

private:
    TimeManager() : current_frame_time_(last_frame_time_ = timer_.tick()) {}
    TimeManager(const TimeManager&) = delete;

    wheel::Timer timer_;

    wheel::time_t dt_ = 0.0f;
    wheel::time_t last_frame_time_ = 0;
    wheel::time_t current_frame_time_ = 0;
};

}  // namespace core
