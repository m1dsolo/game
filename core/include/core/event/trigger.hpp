#pragma once

#include <ecs/entity.hpp>

namespace core {

struct TriggerEnterEvent {
    wheel::Entity trigger;
    wheel::Entity target;
};

struct TriggerStayEvent {
    wheel::Entity trigger;
    wheel::Entity target;
};

struct TriggerExitEvent {
    wheel::Entity trigger;
    wheel::Entity target;
};

}  // namespace core
