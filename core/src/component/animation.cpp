#include <core/component/animation.hpp>
#include <core/manager/animation.hpp>

namespace core {

AnimationComponent::AnimationComponent(const std::string& key)
    : animation(&AnimationManager::instance().get(key)) {}

}  // namespace core
