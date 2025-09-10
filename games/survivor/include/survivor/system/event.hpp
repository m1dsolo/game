#pragma once

#include <core/system/base.hpp>

namespace survivor {

class EventSystem : public core::BaseSystem {
public:
    EventSystem() : BaseSystem("Event") {}

    void update_impl() override;

private:
    void hp_change_event_();
    void death_event_();
};

}  // namespace survivor

