#pragma once

#include <rfl/internal/has_reflector.hpp>

#include <core/component/animation.hpp>

namespace rfl {

template<>
struct Reflector<core::AnimationComponent> {
    struct ReflType {
        wheel::ID name_id;
        wheel::ID state_id;
        float time = 0.f;
    };

    static core::AnimationComponent to(const ReflType& value) {
        auto animation = core::AnimationComponent{value.name_id, value.state_id};
        animation.time = value.time;
        return animation;
    }

    static ReflType from(const core::AnimationComponent& value) {
        return {
            .name_id = value.animation->id.name_id,
            .state_id = value.animation->id.state_id,
            .time = value.time
        };
    }
};

}  // namespace rfl
