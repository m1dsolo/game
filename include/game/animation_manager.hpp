#pragma once

#include <string>
#include <memory>
#include <unordered_map>

#include <SDL3/SDL.h>

#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>

#include <game/animation.hpp>

namespace wheel {

class AnimationManager : public Singleton<AnimationManager> {
    friend class Singleton<AnimationManager>;

public:
    Animations* get_animations(const std::string& name);

private:
    AnimationManager();
    ~AnimationManager() = default;
    AnimationManager(const AnimationManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<Animations>> animations_map_;
};

}  // namespace wheel 
