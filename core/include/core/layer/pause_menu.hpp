#pragma once

#include <core/layer/layer.hpp>

#include <ecs/entity.hpp>

#include <vector>

namespace core {

class PauseMenuLayer : public Layer {
public:
    PauseMenuLayer() : Layer("PauseMenuLayer") {}

    void on_attach() override;
    void on_detach() override;
    void on_show() override;
    void on_hide() override;
    void on_update() override;
    bool on_event(const SDL_Event& event) override;

private:
    inline static std::vector<wheel::Entity> entities_;
    inline static wheel::Entity resume_button_entity_;
    inline static wheel::Entity exit_button_entity_;
    inline static wheel::Entity layout_entity_;
};

}  // namespace core
