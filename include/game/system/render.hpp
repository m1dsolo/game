#pragma once

#include <game/system/base.hpp>

#include <game/ui.hpp>

namespace wheel {

class RenderSystem : public BaseSystem<RenderSystem> {
    friend class BaseSystem<RenderSystem>;

public:
    void execute_impl() override;

private:
    RenderSystem() : BaseSystem(true), ui_(UI::instance()) {}
    ~RenderSystem() = default;
    RenderSystem(const RenderSystem&) = delete;

    UI& ui_;
};

}  // namespace wheel

