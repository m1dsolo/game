#pragma once

#include <wheel/timer.hpp>
#include <wheel/singleton.hpp>

namespace wheel {

struct TimerResource {
    TimerResource();
    ~TimerResource();

    timer_t last;
    timer_t delta = 0;  // us since last update(last frame), value in [min_delta, +inf]
    Timer& timer = Timer::instance();
    timer_t us_per_frame;
    bool pause = false;

    template <typename F>
    void add(int frames, int cnt, F&& f, bool immediately = false) {
        timer.add(frames * us_per_frame, cnt, std::forward<F>(f), immediately);
    }
};

}  // namespace wheel
