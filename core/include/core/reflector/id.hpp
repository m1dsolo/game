#pragma once

#include <rfl/internal/has_reflector.hpp>

#include <wheel/id.hpp>

namespace rfl {

template<>
struct Reflector<wheel::ID> {
    using ReflType = uint32_t;

    static wheel::ID to(const ReflType& value) {
        return static_cast<wheel::ID>(value);
    }

    static ReflType from(const wheel::ID& value) {
        return static_cast<ReflType>(value);
    }
};

}  // namespace rfl
