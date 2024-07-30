#pragma once

#include <game/system/base.hpp>

namespace wheel {

class TimerSystem : public BaseSystem<TimerSystem> {
    friend class BaseSystem<TimerSystem>;

public:
    void execute_impl() override;
     
private:
    TimerSystem() : BaseSystem(true) {}
    ~TimerSystem() = default;
    TimerSystem(const TimerSystem&) = delete;

    void update();
};

}  // namespace wheel
