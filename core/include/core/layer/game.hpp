#pragma once

#include <core/layer/layer.hpp>

namespace core {

class GameLayer : public Layer {
public:
    GameLayer() : Layer("GameLayer") {}

    void on_attach() override;
    void on_detach() override;
    bool on_event(const SDL_Event& event) override;
};

}  // namespace core
