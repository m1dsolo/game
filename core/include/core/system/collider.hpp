#pragma once

#include <core/system/base.hpp>

namespace core {

class ColliderSystem : public BaseSystem {
public:
    ColliderSystem() : BaseSystem("Collider") {}

    void update_impl() override;
};

}  // namespace core
