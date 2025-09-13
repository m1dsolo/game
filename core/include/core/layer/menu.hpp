#pragma once

#include <core/layer/layer.hpp>

namespace core {

class MenuLayer : public Layer {
public:
    // void on_attach() override;
    // void on_detach() override;
    void on_render() override;
    bool on_event(const SDL_Event& event) override;
};

}  // namespace core
