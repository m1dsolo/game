#include <game/layer/exit.hpp>

#include <game/global.hpp>
#include <game/ui.hpp>
#include <game/event.hpp>

namespace wheel {

ExitLayer::ExitLayer() {
    w = text_field_w_ * 1 + padding_ * 2;
    h = text_field_h_ * 2 + padding_ * 3;
    texture_ = sdl.create_texture(w, h, sdl.BLACK, SDL_TEXTUREACCESS_TARGET);
    sdl.set_blend_mode(texture_, SDL_BLENDMODE_NONE);

    sdl.set_target(texture_);

    // quit
    float quit_x = padding_;
    float quit_y = padding_;
    SDL_FRect quit_dst = {quit_x, quit_y, text_field_w_, text_field_h_};
    sdl.draw_rect(&quit_dst, sdl.WHITE);

    // quit text
    sdl.draw_text("quit", &quit_dst, sdl.BLACK, true);
    
    // cancel
    float cancel_x = padding_;
    float cancel_y = padding_ * 2 + text_field_h_;
    SDL_FRect cancel_dst = {cancel_x, cancel_y, text_field_w_, text_field_h_};
    sdl.draw_rect(&cancel_dst, sdl.WHITE);

    // cancel text
    sdl.draw_text("cancel", &cancel_dst, sdl.BLACK, true);

    sdl.set_target(nullptr);
}

void ExitLayer::on_attach() {
    game_resource.paused = true;
    timer.pause();
}

void ExitLayer::on_detach() {
    game_resource.paused = false;
    timer.resume();
}

void ExitLayer::on_update() {
}

void ExitLayer::on_render() {
    SDL_FRect dst = {(config_resource.w - w) / 2, (config_resource.h - h) / 2, w, h};
    sdl.render(texture_, nullptr, &dst);
}

bool ExitLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_1: {
                    game_resource.running = false;
                    return true;
                }
                case SDLK_2: {
                    ecs.emplace_event<PopLayerEvent>();
                    return true;
                }
            }
            return true;
        }
    }
    return false;
}

}  // namespace wheel

