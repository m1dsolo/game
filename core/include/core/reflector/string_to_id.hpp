#pragma once

#include <rfl/internal/has_reflector.hpp>

#include <wheel/id.hpp>

namespace rfl {

template<>
struct Reflector<wheel::ID> {
    using ReflType = std::string;

    static wheel::ID to(const ReflType& value) {
        return static_cast<wheel::ID>(value);
    }
};

}  // namespace rfl
