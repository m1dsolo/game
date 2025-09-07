#pragma once

#include <core/system/base.hpp>

namespace core {

class RenderSystem : public BaseSystem {
public:
    RenderSystem() : BaseSystem("Render") {}

    void update_impl() override;

private:
    void render_texture_();

    static const unsigned int MAX_LAYER = 2;  // begin from 0
};

}  // namespace core
