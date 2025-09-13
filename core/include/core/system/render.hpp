#pragma once

#include <core/system/base.hpp>

namespace core {

class RenderSystem : public BaseSystem {
public:
    RenderSystem() : BaseSystem("Render") {}

    void update_impl() override;
};

}  // namespace core
