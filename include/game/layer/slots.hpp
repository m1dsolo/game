#pragma once

#include <vector>

#include <game/layer/layer.hpp>

namespace wheel {

class SlotsLayer final : public Layer {
public:
    // if hotbar is true, the slots will be displayed at bottom
    // else is displayed at center as for inventory
    SlotsLayer(bool hotbar);
    ~SlotsLayer() = default;
    SlotsLayer(const SlotsLayer&) = delete;

    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    void on_render() override;
    bool on_event(const SDL_Event& event) override;

private:
    int n;
    int m;
    SDL_Texture* texture_;
    SDL_FRect main_rect_;
    std::vector<SDL_FRect> slot_rects_;
    std::vector<SDL_FRect> item_rects_;

    int slot_w = 80;
    int slot_h = 80;
    int slot_padding = 5;
    int item_w = 48;
    int item_h = 48;
    int w;
    int h;
    int mouse_target_idx = -1;
    int base_idx = 1;  // 1 for hotbar, 11 for inventory
};

}  // namespace wheel

