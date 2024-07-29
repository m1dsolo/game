#pragma once

#include <vector>

#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>

#include <game/layer/layer.hpp>

namespace wheel {

class CardLayer final : public Layer, public Singleton<CardLayer> {
    friend class Singleton<CardLayer>;

public:
    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    void on_render() override;
    bool on_event(const SDL_Event& event) override;

private:
    CardLayer();
    ~CardLayer() = default;
    CardLayer(const CardLayer&) = delete;

    void choose(int idx);

    SDL_Texture* bg_texture_;
    SDL_Texture* text_texture_;

    std::vector<size_t> card_idxs;
    Vector2D<double> positions[3];
    SDL_FRect card_dsts[3];

    int selection_ = -1;

    int card_w_ = 300;
    int card_h_ = 500;
};

}  // namespace wheel

