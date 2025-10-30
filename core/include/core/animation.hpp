#pragma once

#include <core/manager/sprite.hpp>

#include <wheel/id.hpp>

#include <vector>
#include <functional>

namespace core {

// "{name}-{state}" e.g. "slime-idle"
struct Animation {
    struct ID {
        ID(wheel::ID name_id = "", wheel::ID state_id = "")
            : name_id(name_id), state_id(state_id) {}

        bool operator==(const ID& other) const {
            return name_id == other.name_id && state_id == other.state_id;
        }

        wheel::ID name_id;
        wheel::ID state_id;
    } id;

    std::vector<Sprite> sprites;
    float duration = 1.f;
    bool loop = true;
};

}  // namespace core

namespace std {
    template<> struct hash<core::Animation::ID> {
        size_t operator()(const core::Animation::ID& id) const {
            return (static_cast<size_t>(id.name_id) << 32) | static_cast<size_t>(id.state_id);
        }
    };
}
