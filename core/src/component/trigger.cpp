#include <core/component/trigger.hpp>

namespace core {

TriggerComponent::TriggerComponent(
    std::variant<wheel::Rect<float>, wheel::Circle<float>> shape,
    bool dynamic,
    std::function<void(wheel::Entity, wheel::Entity)> on_enter,
    std::function<void(wheel::Entity, wheel::Entity)> on_stay,
    std::function<void(wheel::Entity, wheel::Entity)> on_exit
) : ColliderComponent{shape, dynamic},
    on_enter(on_enter),
    on_stay(on_stay),
    on_exit(on_exit),
    stay_entities() {}

}  // namespace core
