#pragma once

#include <core/component/sprite.hpp>

#include <vector>
#include <functional>

namespace core {

// "{name}-{state}" e.g. "slime-idle"
struct Animation {
    struct ID {
        ID(const std::string& name = "", const std::string& state = "")
            : name(name), state(state), key(name + "-" + state) {}

        bool operator==(const ID& other) const {
            return key == other.key;
        }

        std::string name;
        std::string state;
        std::string key;
    } id;

    std::vector<Sprite> sprites;
    float duration = 1.f;
    bool loop = true;
};

}  // namespace core

namespace std {
    template<> struct hash<core::Animation::ID> {
        size_t operator()(const core::Animation::ID& id) const {
            return hash<std::string>()(id.key);
        }
    };
}
