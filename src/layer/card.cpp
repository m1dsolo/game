#include <game/layer/card.hpp>

#include <game/global.hpp>
#include <game/card.hpp>
#include <game/event.hpp>
#include <game/game_manager.hpp>
#include <game/ui.hpp>

#include <game/component/animation.hpp>

namespace wheel {

CardLayer::CardLayer() {
    bg_texture_ = sdl.create_texture(config_resource.w, config_resource.h, sdl.BLACK, SDL_TEXTUREACCESS_TARGET, SDL_PIXELFORMAT_RGBA8888);
    sdl.set_blend_mode(bg_texture_, SDL_BLENDMODE_BLEND);
    sdl.set_alpha_mod(bg_texture_, 0);

    SDL_Texture* card_bg_texture = sdl.create_texture(card_w_, card_h_, sdl.PINK);

    positions[1] = {(config_resource.w) / 2., (config_resource.h) / 2.};
    positions[0] = {positions[1].x - card_w_ - 200, positions[1].y};
    positions[2] = {positions[1].x + card_w_ + 200, positions[1].y};

    sdl.set_target(bg_texture_);
    sdl.set_alpha_mod(bg_texture_, 255);
    for (int i = 0; i < 3; i++) {
        card_dsts[i] = {(float)(positions[i].x - card_w_ / 2.), (float)(positions[i].y - card_h_ / 2.), (float)card_w_, (float)card_h_};
        sdl.render(card_bg_texture, nullptr, &card_dsts[i]);
    }
    sdl.set_target(nullptr);

    sdl.destroy(card_bg_texture);
}

void CardLayer::on_attach() {
    GameManager::instance().pause();

    text_texture_ = sdl.create_texture(config_resource.w, config_resource.h, sdl.BLACK, SDL_TEXTUREACCESS_TARGET, SDL_PIXELFORMAT_RGBA8888);
    sdl.set_blend_mode(text_texture_, SDL_BLENDMODE_BLEND);
    sdl.set_alpha_mod(text_texture_, 0);

    card_idxs.clear();
    sdl.set_target(text_texture_);
    sdl.set_alpha_mod(text_texture_, 255);
    for (int i = 0; i < 3; i++) {
        // get card idx
        size_t idx;
        while (true) {
            idx = game_resource.random.uniform(0, (int)CardFactory::instance().size() - 1);
            bool found = false;
            for (auto card_idx : card_idxs) {
                if (card_idx == idx) {
                    found = true;
                    break;
                }
            }
            if (found) {
                continue;
            }

            card_idxs.emplace_back(idx);
            break;
        }

        // add card text texture
        const std::string& name = CardFactory::instance().get(idx)->name;
        SDL_FRect text_dst = {(float)positions[i].x, (float)positions[i].y, 0, 0};
        sdl.draw_text(name, &text_dst, sdl.BLACK, true, 0, 0, card_w_ - 100);
    }
    sdl.set_target(nullptr);
}

void CardLayer::on_detach() {
    sdl.destroy(text_texture_);
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
                    choose(card_idxs[0]);
                    break;
                }
                case SDLK_2: {
                    choose(card_idxs[1]);
                    break;
                }
                case SDLK_3: {
                    choose(card_idxs[2]);
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
                        choose(card_idxs[selection_]);
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
    CardFactory::instance().get(idx)->execute();
    UI::instance().del<CardLayer>();
    GameManager::instance().resume();
}

}  // namespace wheel
