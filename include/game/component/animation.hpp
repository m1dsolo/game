#pragma once

#include <game/animation.hpp>

#include <game/animation_manager.hpp>

namespace wheel {

struct AnimationComponent {
    AnimationComponent(const std::string& name)
        : animations(AnimationManager::instance().get_animations(name)), frames(animations->frames()) {}
    Animations* animations;

    int idx = 0;  // current frame
    int counter = 0;
    int max_counter = 30;
    int frames = 0;  // for cache
    Animations::State state = Animations::State::NORMAL;
};

struct SwitchAnimationStateComponent {
    Animations::State new_state = Animations::State::NORMAL;
    int max_counter = -1;  // -1 for no change
    int duration = -1;  // -1 for infinite
};

}  // namespace wheel
