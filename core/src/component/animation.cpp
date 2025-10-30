#include <core/component/animation.hpp>
#include <core/manager/animation.hpp>

namespace core {

AnimationComponent::AnimationComponent(wheel::ID name_id, wheel::ID state_id)
    : animation(&AnimationManager::instance().get({name_id, state_id})) {}

}  // namespace core
