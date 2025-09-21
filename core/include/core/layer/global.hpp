#pragma once

#include <core/layer/layer.hpp>

namespace core {

class GlobalLayer : public Layer {
public:
    GlobalLayer() : Layer("GlobalLayer") {}

    // void on_attach() override;
    // void on_detach() override;
    // void on_update() override;
    bool on_event(const SDL_Event& event) override;
};

}  // namespace core
