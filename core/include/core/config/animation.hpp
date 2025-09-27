#pragma once

#include <core/manager/sprite.hpp>

#include <vector>

namespace core {

struct AnimationConfig {
    std::string state;

    std::optional<float> duration;
    std::optional<bool> loop;
    std::optional<std::pair<int, int>> size;
    std::optional<std::pair<int, int>> start;
    std::optional<std::pair<int, int>> step;
    std::optional<int> count;
};

struct AnimationsConfig {
    std::string name;
    std::string path;
    std::vector<AnimationConfig> animations;

    std::optional<float> duration;
    std::optional<bool> loop;
    std::optional<std::pair<int, int>> size;
    std::optional<std::pair<int, int>> start;
    std::optional<std::pair<int, int>> step;
    std::optional<int> count;
};

}  // namespace core
