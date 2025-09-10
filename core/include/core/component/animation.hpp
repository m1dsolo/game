#pragma once

#include <core/animation.hpp>
#include <core/manager/animation.hpp>

namespace core {

struct AnimationComponent {
    AnimationComponent(const std::string& key = "");

    const Animation* animation;
    float time = 0.f;
};

}  // namespace core
