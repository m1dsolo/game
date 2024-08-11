#pragma once

#include <game/animation.hpp>

#include <game/animation_manager.hpp>

namespace wheel {

struct AnimationComponent {
    AnimationComponent(const std::string& name) : animations(AnimationManager::instance().get_animations(name)) {}
    Animations* animations;

    int idx = 0;  // current frame
    int counter = 0;
    Animations::Type type = Animations::Type::NORMAL;
};

}  // namespace wheel
