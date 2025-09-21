#pragma once

#include <core/layer/layer.hpp>

#include <ecs/entity.hpp>

#include <vector>

namespace core {

class MenuLayer : public Layer {
public:
    void on_attach() override;
    void on_detach() override;
    bool on_event(const SDL_Event& event) override;

protected:
    MenuLayer(const std::string& name) : Layer(name) {}

    std::vector<wheel::Entity> button_entities_;
    int index_ = 0;

private:
    void hover_button_(int index_delta);
};

}  // namespace core
