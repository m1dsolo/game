#pragma once

#include <wheel/singleton.hpp>

namespace wheel {

class AnimationSystem : public Singleton<AnimationSystem> {
    friend class Singleton<AnimationSystem>;

public:
    void execute();

private:
    AnimationSystem() = default;
    ~AnimationSystem() = default;
    AnimationSystem(const AnimationSystem&) = delete;

    void update_animation_counter();
    void update_texture();
};

}  // namespace wheel
