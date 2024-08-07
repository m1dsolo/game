#pragma once

#include <wheel/singleton.hpp>

#include <game/layer/layer.hpp>

namespace wheel {

class HotBarLayer final : public Layer, public Singleton<HotBarLayer> {
    friend class Singleton<HotBarLayer>;

public:
    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    void on_render() override;
    bool on_event(const SDL_Event& event) override;

private:
    HotBarLayer();
    ~HotBarLayer() = default;
    HotBarLayer(const HotBarLayer&) = delete;

    static const int SLOT_COUNT = 10;

    SDL_Texture* texture_;
    SDL_FRect main_rect_;
    SDL_FRect slot_rects_[SLOT_COUNT];
    SDL_FRect item_rects_[SLOT_COUNT];

    int slot_w = 80;
    int slot_h = 80;
    int slot_padding = 5;
    int item_w = 48;
    int item_h = 48;
    int w;
    int h;
    int mouse_target_idx = -1;
};

}  // namespace wheel


