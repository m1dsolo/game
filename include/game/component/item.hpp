#pragma once

#include <string>
#include <variant>
#include <memory>

#include <game/item/item.hpp>

namespace wheel {

struct ItemComponent {
    // std::string: item name to create item
    // std::shared_ptr<Item>: item to pick
    std::variant<std::string, std::shared_ptr<Item>> data;
    int count = 1;
};

}  // namespace wheel
