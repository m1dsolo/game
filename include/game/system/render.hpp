#pragma once

#include <game/system/base.hpp>

namespace wheel {

class RenderSystem : public BaseSystem<RenderSystem> {
    friend class BaseSystem<RenderSystem>;

public:
    void execute_impl() override;

private:
    RenderSystem() : BaseSystem(true) {}
    ~RenderSystem() = default;
    RenderSystem(const RenderSystem&) = delete;

    void update_health_bar_texture();
};

}  // namespace wheel

