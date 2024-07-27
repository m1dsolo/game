#pragma once

#include <wheel/singleton.hpp>

namespace wheel {

class TimerSystem : public Singleton<TimerSystem> {
    friend class Singleton<TimerSystem>;

public:
    void execute();
     
private:
    TimerSystem() = default;
    ~TimerSystem() = default;
    TimerSystem(const TimerSystem&) = delete;

    void update();
};

}  // namespace wheel
