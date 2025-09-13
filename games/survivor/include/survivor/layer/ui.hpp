#pragma once

#include <core/layer/layer.hpp>

#include <ecs/entity.hpp>

namespace survivor {

class UILayer : public core::Layer {
public:
    void on_attach() override;
    // void on_detach() override;
    void on_render() override;
    bool on_event(const SDL_Event& event) override;

private:
    wheel::Entity player_entity_;
};

}  // namespace survivor
