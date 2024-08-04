#pragma once

#include <vector>
#include <functional>

#include <wheel/geometry.hpp>

#include <game/card.hpp>
#include <game/layer/layer.hpp>

namespace wheel {

class CardLayer final : public Layer {
public:
    CardLayer(bool player_cards = true);
    ~CardLayer() = default;
    CardLayer(const CardLayer&) = delete;

    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    void on_render() override;
    bool on_event(const SDL_Event& event) override;

    void set_detach_callback(std::function<void()> callback) {
        detach_callback_ = callback;
    }

private:
    void choose(int idx);

    SDL_Texture* bg_texture_;
    SDL_Texture* text_texture_;

    std::vector<size_t> card_idxs;
    Vector2D<double> positions[3];
    SDL_FRect card_dsts[3];

    int selection_ = -1;

    int card_w_ = 300;
    int card_h_ = 500;

    const std::vector<std::shared_ptr<Card>>& cards_;
    std::function<void()> detach_callback_;
};

}  // namespace wheel

