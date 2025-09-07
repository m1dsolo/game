#pragma once

#include <core/system/base.hpp>

#include <wheel/geometry.hpp>

namespace core {

struct Animation;

class AnimationSystem : public BaseSystem {
public:
    AnimationSystem() : BaseSystem("Animation") {}

    void update_impl() override;

private:
    static std::string direction_to_string_(const wheel::Vector2D<float>& direction);
};

}  // namespace core
