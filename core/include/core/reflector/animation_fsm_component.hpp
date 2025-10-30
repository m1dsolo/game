#pragma once

#include <rfl/internal/has_reflector.hpp>

#include <core/component/animation_fsm.hpp>

namespace rfl {

template<>
struct Reflector<core::AnimationFSMComponent> {
    struct ReflType {
        wheel::ID id;
        wheel::ID current_state_id = "start";
    };

    static core::AnimationFSMComponent to(const ReflType& value) {
        auto animation_fsm = core::AnimationFSMComponent{value.id};
        animation_fsm.current_state_id = value.current_state_id;
        return animation_fsm;
    }

    static ReflType from(const core::AnimationFSMComponent& value) {
        return ReflType{
            value.fsm->id,
            value.current_state_id
        };
    }
};

}  // namespace rfl
