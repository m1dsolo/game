#pragma once

#include <core/animation.hpp>
#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>

#include <string>
#include <unordered_map>

#include <iostream>

namespace core {

class AnimationManager : public wheel::Singleton<AnimationManager> {
    friend wheel::Singleton<AnimationManager>;

public:
    void set(const std::string& key, const Animation& animation) {
        std::cout << "animation_set: " << key << " " << animation.sprites.size() << " " << animation.duration << " " << animation.loop << std::endl;
        animation_map_[key] = animation;
    }

    const Animation& get(const std::string& key) const {
        if (animation_map_.find(key) == animation_map_.end()) {
            std::cout << "not found animation: " << key << "!!!!!" << std::endl;
            return animation_map_.at("");
        }
        // std::cout << "animation_get: " << key << std::endl;
        return animation_map_.at(key);
    }

private:
    AnimationManager();
    AnimationManager(const AnimationManager&) = delete;

    std::unordered_map<std::string, Animation> animation_map_;
};

}  // namespace core
