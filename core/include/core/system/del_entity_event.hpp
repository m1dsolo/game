#pragma once

#include <core/system/base.hpp>

namespace core {

class DelEntityEventSystem : public BaseSystem {
public:
    DelEntityEventSystem() : BaseSystem("DelEntityEvent") {}

    void update_impl() override;
};

}  // namespace core
