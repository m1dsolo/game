#pragma once

#include <game/system/base.hpp>

namespace wheel {

class EventSystem : public BaseSystem<EventSystem> {
    friend class BaseSystem<EventSystem>;

public:
    void startup();

private:
    EventSystem() : BaseSystem(true) {}
    ~EventSystem() = default;
    EventSystem(const EventSystem&) = delete;
};

}  // namespace wheel
