#pragma once

#include <core/system/base.hpp>

namespace survivor {

class RangeAttackSystem : public core::BaseSystem {
public:
    RangeAttackSystem() : BaseSystem("RangeAttack") {}

    void update_impl() override;

private:
    void shoot_();
    void collide_();
};

}  // namespace survivor

