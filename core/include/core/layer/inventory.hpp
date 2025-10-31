#pragma once

#include <core/layer/layer.hpp>

#include <ecs/entity.hpp>

namespace core {

class InventoryLayer : public Layer {
public:
    InventoryLayer() {}

    void on_register() override;
    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    bool on_event(const SDL_Event& event) override;

private:
    wheel::Entity selected_slot_border_;
    int selected_idx_[2] = { 0, 0 };

    void select_slot_(int delta_x, int delta_y);
};

}  // namespace core
