#pragma once

#include <core/system/base.hpp>

namespace core {

class TrackSystem : public core::BaseSystem {
public:
    TrackSystem() : BaseSystem("Track") {}

    void update_impl() override;
};

}  // namespace core
