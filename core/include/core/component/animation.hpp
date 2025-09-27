#pragma once

#include <core/animation.hpp>

namespace core {

struct AnimationComponent {
    AnimationComponent(const Animation::ID& id = {});

    const Animation* animation;
    float time = 0.f;
};

}  // namespace core
