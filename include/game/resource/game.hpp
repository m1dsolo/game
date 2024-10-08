#pragma once

#include <string>
#include <filesystem>

#include <wheel/timer.hpp>
#include <wheel/random.hpp>
#include <wheel/geometry.hpp>

namespace wheel {

struct GameResource {
    GameResource();
    ~GameResource();

    std::unordered_map<std::string, std::any> cache;

    std::filesystem::path path = std::filesystem::current_path();

    Random random;
    bool second_flag = false;  // first frame of every second will set true
    long long seconds = 0;
};

}  // namespace wheel

