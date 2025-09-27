#include <core/component/animation.hpp>
#include <core/manager/animation.hpp>

namespace core {

AnimationComponent::AnimationComponent(const Animation::ID& id)
    : animation(&AnimationManager::instance().get(id)) {}

}  // namespace core
