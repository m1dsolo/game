#pragma once

#include <string>

namespace core {

struct AddLayerEvent {
    std::string layer_name;
};

struct RemoveLayerEvent {};

}  // namespace core
