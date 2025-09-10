#pragma once

#include <core/system/base.hpp>

#include <ecs/entity.hpp>

namespace core {

class TriggerSystem : public BaseSystem {
public:
    TriggerSystem() : BaseSystem("Trigger") {}

    void update_impl() override;
};

}  // namespace core
