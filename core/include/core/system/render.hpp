#pragma once

#include <core/system/base.hpp>

namespace core {

class RenderSystem : public BaseSystem {
public:
    RenderSystem() : BaseSystem("Render") {}

    void update_impl() override;

private:
    void render_texture_();

    // 0. map
    // 1. player, enemy
    // 2. hp_bar, bullet
    static const unsigned int MAX_LAYER = 2;
};

}  // namespace core
