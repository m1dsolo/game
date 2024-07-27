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

    bool running = true;

    std::unordered_map<std::string, std::any> cache;

    std::string path = std::filesystem::current_path();

    // game
    bool paused = false;

    Random random;
};

}  // namespace wheel

