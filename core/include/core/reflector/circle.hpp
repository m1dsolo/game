#pragma once

#include <rfl/internal/has_reflector.hpp>

#include <wheel/geometry.hpp>

namespace rfl {

template<typename T>
struct Reflector<wheel::Circle<T>> {
    struct ReflType {
        T radius;
        T center_x;
        T center_y;
    };

    static wheel::Circle<T> to(const ReflType& value) {
        return { value.radius, value.center_x, value.center_y };
    }

    static ReflType from(const wheel::Circle<T>& value) {
        return { value.radius, value.center[0], value.center[1] };
    }
};

}  // namespace rfl
