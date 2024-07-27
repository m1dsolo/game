#include <game/layer/card.hpp>

#include <game/global.hpp>
#include <game/card.hpp>
#include <game/event.hpp>

#include <game/component/animation.hpp>

namespace wheel {

CardLayer::CardLayer() {
    int card_w = config_resource.card_w, card_h = config_resource.card_h;
    bg_texture_ = sdl.create_texture(config_resource.w, config_resource.h, sdl.BLACK, SDL_TEXTUREACCESS_TARGET, SDL_PIXELFORMAT_RGBA8888);
    sdl.set_blend_mode(bg_texture_, SDL_BLENDMODE_BLEND);
    sdl.set_alpha_mod(bg_texture_, 0);

    SDL_Texture* card_bg_texture = sdl.create_texture(card_w, card_h, sdl.PINK);

    positions[1] = {(config_resource.w) / 2., (config_resource.h) / 2.};
    positions[0] = {positions[1].x - card_w - 200, positions[1].y};
    positions[2] = {positions[1].x + card_w + 200, positions[1].y};

    sdl.set_target(bg_texture_);
    sdl.set_alpha_mod(bg_texture_, 255);
    for (int i = 0; i < 3; i++) {
        SDL_FRect dst = {static_cast<float>(positions[i].x - card_w / 2.), static_cast<float>(positions[i].y - card_h / 2.), static_cast<float>(card_w), static_cast<float>(card_h)};
        sdl.render(card_bg_texture, nullptr, &dst);
    }
    sdl.set_target(nullptr);

    sdl.destroy(card_bg_texture);
}

void CardLayer::on_attach() {
    if (game_resource.paused) {
        return;
    }
    game_resource.paused = true;
    timer.pause();

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
        sdl.draw_text_in_mid(name, &text_dst, sdl.BLACK);
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
}

bool CardLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_1: {
                    CardFactory::instance().get(card_idxs[0])->execute();
                    ecs.emplace_event<PopLayerEvent>();
                    game_resource.paused = false;
                    timer.resume();
                    return true;
                }
                case SDLK_2: {
                    CardFactory::instance().get(card_idxs[1])->execute();
                    ecs.emplace_event<PopLayerEvent>();
                    game_resource.paused = false;
                    timer.resume();
                    return true;
                }
                case SDLK_3: {
                    CardFactory::instance().get(card_idxs[2])->execute();
                    ecs.emplace_event<PopLayerEvent>();
                    game_resource.paused = false;
                    timer.resume();
                    return true;
                }
            }
            return true;
        }
    }
    return false;
}

}  // namespace wheel
