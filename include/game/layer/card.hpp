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
    ~CardLayer();
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
    SDL_Texture* rarity_textures_[6];
    SDL_Texture* text_texture_;

    Vector2D<float> positions[3];
    SDL_FRect card_dsts[3];

    int selection_ = -1;

    int card_w_ = 300;
    int card_h_ = 500;

    std::vector<Card*> cards_;
    std::function<void()> detach_callback_;

    bool player_cards_;
};

}  // namespace wheel

