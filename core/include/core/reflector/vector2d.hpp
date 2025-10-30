#pragma once

#include <rfl/internal/has_reflector.hpp>

#include <wheel/geometry.hpp>

#include <array>

namespace rfl {

template<typename T>
struct Reflector<wheel::Vector2D<T>> {
    using ReflType = std::array<T, 2>;

    static wheel::Vector2D<T> to(const ReflType& value) {
        return { value[0], value[1] };
    }

    static ReflType from(const wheel::Vector2D<T>& value) {
        return { value[0], value[1] };
    }
};

}  // namespace rfl
