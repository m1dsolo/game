#pragma once

#include <core/system/base.hpp>

namespace core {

class TimeSystem : public BaseSystem {
public:
    TimeSystem() : BaseSystem("Time") {}

    void update_impl() override;
};

}  // namespace core
