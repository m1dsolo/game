#pragma once

#include <core/animation.hpp>
#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>

#include <unordered_map>

namespace core {

class AnimationManager : public wheel::Singleton<AnimationManager> {
    friend wheel::Singleton<AnimationManager>;

public:
    void set(const Animation& clip);

    const Animation& get(const Animation::ID& id);

private:
    AnimationManager();
    AnimationManager(const AnimationManager&) = delete;

    std::unordered_map<Animation::ID, Animation> clip_map_;
};

}  // namespace core
