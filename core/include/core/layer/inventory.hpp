#pragma once

#include <core/layer/layer.hpp>

namespace core {

class InventoryLayer : public core::Layer {
public:
    InventoryLayer() {}

    void on_register() override;
    void on_show() override;
    void on_hide() override;
    void on_update() override;
    bool on_event(const SDL_Event& event) override;
};

}  // namespace core
