#pragma once

#include <functional>

namespace wheel {

struct ContinuousActionComponent {
    std::function<bool()> action;
};

}  // namespace wheel
