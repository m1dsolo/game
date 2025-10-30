#pragma once

#include <rfl/internal/has_reflector.hpp>

#include <core/component/sprite.hpp>

namespace rfl {

template<>
struct Reflector<core::SpriteComponent> {
    struct ReflType {
        wheel::ID id;
        bool flip;
        SDL_FColor color;
    };

    static core::SpriteComponent to(const ReflType& value) {
        auto sprite = core::SpriteComponent(value.id);
        sprite.flip = value.flip;
        sprite.color = value.color;
        return sprite;
    }

    static ReflType from(const core::SpriteComponent& value) {
        return ReflType{
            value.id,
            value.flip,
            value.color,
        };
    }
};

}  // namespace rfl
