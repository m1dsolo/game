#pragma once

#include <core/layer/layer.hpp>

#include <ecs/entity.hpp>

namespace core {

class MainMenuLayer : public Layer {
public:
    MainMenuLayer() {}

    void on_attach() override;
    void on_detach() override;
    void on_show() override;
    void on_hide() override;
    void on_update() override;
    bool on_event(const SDL_Event& event) override;

private:
    wheel::Entity new_button_entity_;
    wheel::Entity load_button_entity_;
    wheel::Entity exit_button_entity_;
    wheel::Entity layout_entity_;
};

}  // namespace core
