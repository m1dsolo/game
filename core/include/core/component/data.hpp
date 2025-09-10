#pragma once

#include <string>
#include <unordered_map>
#include <any>

namespace core {

struct DataComponent {
    std::unordered_map<std::string, std::any> data;
};

}  // namespace core
