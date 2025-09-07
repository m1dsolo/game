#pragma once

#include <core/manager/sprite.hpp>

#include <vector>

namespace core {

struct RangeConfig {
    std::optional<std::pair<int, int>> start;
    std::optional<std::pair<int, int>> step;
    std::optional<int> count;
};

struct ClipConfig {
    std::string direction;
    RangeConfig range;
};

struct AnimationConfig {
    std::string name;
    std::vector<ClipConfig> clips;
};

struct AnimationsConfig {
    std::string id;
    std::string path;
    float duration;
    bool loop;
    std::pair<int, int> size;
    RangeConfig range;
    std::vector<AnimationConfig> animations;
};

}  // namespace core
