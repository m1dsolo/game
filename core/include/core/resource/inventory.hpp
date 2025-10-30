#pragma once

#include <sdl/sdl.hpp>

#include <vector>

namespace core {

struct InventoryResource {
    inline static constexpr int slot_nums[2] = { 10, 5 };
    inline static std::vector<std::vector<SDL_FRect>> slot_rects;
};

}  // namespace core
