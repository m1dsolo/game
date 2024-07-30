#pragma once

#include <game/system/base.hpp>

namespace wheel {

class AnimationSystem : public BaseSystem<AnimationSystem> {
    friend class BaseSystem<AnimationSystem>;

public:
    void execute_impl() override;

private:
    AnimationSystem() : BaseSystem(true) {}
    ~AnimationSystem() = default;
    AnimationSystem(const AnimationSystem&) = delete;

    void update_animation_counter();
    void update_texture();
};

}  // namespace wheel
