#include <core/component/animation_fsm.hpp>
#include <core/manager/animation_fsm.hpp>

namespace core {

AnimationFSMComponent::AnimationFSMComponent(const std::string& key)
    : fsm(&AnimationFSMManager::instance().get(key)) {}

}  // namespace core
