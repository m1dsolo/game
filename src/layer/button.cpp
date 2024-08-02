#include <game/layer/button.hpp>

#include <game/global.hpp>

namespace wheel {

Button::Button(const std::string& text, int padding, std::function<void()> func) : func_(func) {
    // create text
    auto text_texture = sdl.create_texture(text, sdl.BLACK);
    auto [text_w, text_h] = sdl.get_texture_size(text_texture);

    texture_ = sdl.create_texture(text_w + padding * 2, text_h + padding * 2, sdl.BLACK, SDL_TEXTUREACCESS_TARGET);

    SDL_FRect dst = {(float)padding, (float)padding, (float)text_w, (float)text_h};

    sdl.set_target(texture_);
    sdl.draw_rect(nullptr, sdl.WHITE);
    sdl.render(text_texture, nullptr, &dst);
    sdl.set_target(nullptr);

    sdl.destroy(text_texture);
}


void Button::on_attach() {
}

void Button::on_detach() {
}

void Button::on_update() {
}

void Button::on_render() {
    sdl.render(texture_, nullptr, &dst_);

    // render hover
    if (hover_) {
        sdl.draw_border(&dst_, 5, sdl.GRAY);
    }
}

bool Button::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_MOUSE_MOTION: {
            float x = event.motion.x, y = event.motion.y;
            if (x >= dst_.x && x <= dst_.x + dst_.w &&
                y >= dst_.y && y <= dst_.y + dst_.h) {
                hover_ = true;
                return true;
            } else {
                hover_ = false;
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            float x = event.motion.x, y = event.motion.y;
            switch (event.button.button) {
                case SDL_BUTTON_LEFT: {
                    if (x >= dst_.x && x <= dst_.x + dst_.w &&
                        y >= dst_.y && y <= dst_.y + dst_.h) {
                        func_();
                        return true;
                    }
                    break;
                }
            }
        }
    }
    return false;
}

std::pair<int, int> Button::size() {
    return sdl.get_texture_size(texture_);
}

}  // namespace wheel
