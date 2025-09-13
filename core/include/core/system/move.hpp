#pragma once

#include <core/system/base.hpp>

namespace core {

class MoveSystem : public BaseSystem {
public:
    MoveSystem() : BaseSystem("Move") {}

    void update_impl() override;
};

}  // namespace core
