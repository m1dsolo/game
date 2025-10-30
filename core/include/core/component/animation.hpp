#pragma once

#include <core/animation.hpp>

namespace core {

struct AnimationComponent {
    AnimationComponent(wheel::ID name_id, wheel::ID state_id = "idle");

    const Animation* animation;
    float time = 0.f;
};

}  // namespace core
