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
};

}  // namespace wheel

