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

    std::string path = std::filesystem::current_path();

    Random random;

    int combat_seconds = 30;
    int enemy_num = 30;
};

}  // namespace wheel

