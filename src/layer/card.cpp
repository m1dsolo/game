#include <game/layer/card.hpp>

#include <game/global.hpp>
#include <game/event.hpp>
#include <game/game_manager.hpp>
#include <game/card_manager.hpp>
#include <game/ui.hpp>

#include <game/component/animation.hpp>

namespace wheel {

CardLayer::CardLayer(bool player_cards) : player_cards_(player_cards) {
    bg_texture_ = sdl.create_texture(config_resource.w, config_resource.h, sdl.BLACK, SDL_TEXTUREACCESS_TARGET, SDL_PIXELFORMAT_RGBA8888);
    sdl.set_blend_mode(bg_texture_, SDL_BLENDMODE_BLEND);
    sdl.set_alpha_mod(bg_texture_, 0);

    for (int i = 0; i < 6; i++) {
        rarity_textures_[i] = sdl.create_texture(card_w_, card_h_, CardManager::rarity2color(i));
    }

    positions[1] = {(float)config_resource.w / 2, (float)config_resource.h / 2};
    positions[0] = {positions[1].x - card_w_ - 200, positions[1].y};
    positions[2] = {positions[1].x + card_w_ + 200, positions[1].y};

    for (int i = 0; i < 3; i++) {
        card_dsts[i] = {(float)(positions[i].x - card_w_ / 2.), (float)(positions[i].y - card_h_ / 2.), (float)card_w_, (float)card_h_};
    }
    sdl.set_target(nullptr);
}

CardLayer::~CardLayer() {
    for (int i = 0; i < 6; i++) {
        sdl.destroy(rarity_textures_[i]);
    }
    sdl.destroy(bg_texture_);
}

void CardLayer::on_attach() {
    GameManager::instance().pause();

    text_texture_ = sdl.create_texture(config_resource.w, config_resource.h, sdl.BLACK, SDL_TEXTUREACCESS_TARGET, SDL_PIXELFORMAT_RGBA8888);
    sdl.set_blend_mode(text_texture_, SDL_BLENDMODE_BLEND);
    sdl.set_alpha_mod(text_texture_, 255);

    sdl.set_target(text_texture_);
    // sdl.set_alpha_mod(text_texture_, 255);
    cards_ = CardManager::instance().get_random_cards(player_cards_);
    for (int i = 0; i < 3; i++) {
        // add rarity color texture
        int rarity = cards_[i]->rarity();
        sdl.render(rarity_textures_[rarity], nullptr, &card_dsts[i]);

        // add card text texture
        const std::string& name = cards_[i]->name();
        SDL_FRect text_dst = {(float)positions[i].x, (float)positions[i].y, 0, 0};
        sdl.draw_text(name, &text_dst, sdl.BLACK, true, 0, 0, card_w_ - 100);
    }
    sdl.set_target(nullptr);
}

void CardLayer::on_detach() {
    sdl.destroy(text_texture_);
    if (detach_callback_) {
        detach_callback_();
    }
}

void CardLayer::on_update() {

}

void CardLayer::on_render() {
    sdl.render(bg_texture_, nullptr, nullptr);
    sdl.render(text_texture_, nullptr, nullptr);

    // render selection
    if (selection_ != -1) {
        sdl.draw_border(&card_dsts[selection_], 4, sdl.RED);
    }
}

bool CardLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_1: {
                    choose(0);
                    break;
                }
                case SDLK_2: {
                    choose(1);
                    break;
                }
                case SDLK_3: {
                    choose(2);
                    break;
                }
            }
            return true;
        }
        case SDL_EVENT_MOUSE_MOTION: {
            float x = event.motion.x, y = event.motion.y;
            selection_ = -1;
            for (int i = 0; i < 3; i++) {
                if (x >= card_dsts[i].x && x <= card_dsts[i].x + card_dsts[i].w && y >= card_dsts[i].y && y <= card_dsts[i].y + card_dsts[i].h) {
                    selection_ = i;
                    break;
                }
            }
            return true;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            switch (event.button.button) {
                case SDL_BUTTON_LEFT: {
                    if (selection_ != -1) {
                        choose(selection_);
                    }
                    break;
                }
            }
            return true;
        }
    }
    return false;
}

void CardLayer::choose(int idx) {
    cards_[idx]->use();
    UI::instance().del<CardLayer>();
    GameManager::instance().resume();
}

}  // namespace wheel
