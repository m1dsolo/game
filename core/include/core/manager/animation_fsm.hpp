#pragma once

#include <core/animation_fsm.hpp>

#include <wheel/singleton.hpp>
#include <wheel/id.hpp>

#include <unordered_map>

namespace core {

class AnimationFSMManager : public wheel::Singleton<AnimationFSMManager> {
    friend wheel::Singleton<AnimationFSMManager>;

public:
    void set(wheel::ID id, const AnimationFSM& fsm) {
        fsm_map_[id] = fsm;
    }

    const AnimationFSM& get(wheel::ID id) const {
        return fsm_map_.at(id);
    }

private:
    AnimationFSMManager();
    AnimationFSMManager(const AnimationFSMManager&) = delete;

    std::unordered_map<wheel::ID, AnimationFSM> fsm_map_;
};

}  // namespace core
