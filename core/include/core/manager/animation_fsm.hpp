#pragma once

#include <core/animation_fsm.hpp>

#include <wheel/singleton.hpp>

#include <unordered_map>
#include <iostream>

namespace core {

class AnimationFSMManager : public wheel::Singleton<AnimationFSMManager> {
    friend wheel::Singleton<AnimationFSMManager>;

public:
    void set(const std::string& key, const AnimationFSM& fsm) {
        std::cout << "[load animation fsm]" << key << std::endl;
        fsm_map_[key] = fsm;
    }

    const AnimationFSM& get(const std::string& key) const {
        return fsm_map_.at(key);
    }

private:
    AnimationFSMManager();
    AnimationFSMManager(const AnimationFSMManager&) = delete;

    std::unordered_map<std::string, AnimationFSM> fsm_map_;
};

}  // namespace core
