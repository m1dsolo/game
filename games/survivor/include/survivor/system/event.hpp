#pragma once

#include <core/system/base.hpp>

#include <ecs/entity.hpp>

namespace survivor {

class EventSystem : public core::BaseSystem {
public:
    EventSystem();

    void update_impl() override;

private:
    void hp_change_event_();
    void death_event_();

    wheel::Entity player_entity_ = wheel::NullEntity;
};

}  // namespace survivor
