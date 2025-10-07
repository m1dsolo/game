#pragma once

#include <core/layer/layer.hpp>

namespace core {

class GlobalLayer : public Layer {
public:
    GlobalLayer() : Layer("GlobalLayer") {}

    bool on_event(const SDL_Event& event) override;
};

}  // namespace core
