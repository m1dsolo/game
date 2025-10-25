#include <core/component/animation_fsm.hpp>
#include <core/manager/animation_fsm.hpp>

namespace core {

AnimationFSMComponent::AnimationFSMComponent(wheel::ID id)
    : fsm(&AnimationFSMManager::instance().get(id)) {}

}  // namespace core
