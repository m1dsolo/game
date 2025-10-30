#pragma once

#include <rfl/internal/has_reflector.hpp>

#include <wheel/geometry.hpp>

#include <array>

namespace rfl {

template<typename T>
struct Reflector<wheel::Rect<T>> {
    using ReflType = std::array<T, 4>;

    static wheel::Rect<T> to(const ReflType& value) {
        return { value[0], value[1], value[2], value[3] };
    }

    static ReflType from(const wheel::Rect<T>& value) {
        return { value.x0, value.y0, value.x1, value.y1 };
    }
};

}  // namespace rfl
