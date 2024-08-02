#include <game/ui.hpp>

#include <game/global.hpp>

namespace wheel {

void UI::update() {
    for (auto layer : layers_) {
        layer->on_update();
    }
    cursor_layer_->on_update();
}

void UI::render() {
    sdl.set_color(sdl.BLACK);
    sdl.clear();

    for (auto layer : layers_) {
        layer->on_render();
    }
    cursor_layer_->on_render();

    sdl.present();
}

void UI::handle_event(const SDL_Event& event) {
    if (cursor_layer_->on_event(event)) {
        return;
    }
    for (auto it = layers_.rbegin(); it != layers_.rend(); ++it) {
        if ((*it)->on_event(event)) {
            return;
        }
    }
}

void UI::pop_front() {
    layers_.front()->on_detach();
    layers_.pop_front();
}

void UI::pop_back() {
    layers_.back()->on_detach();
    layers_.pop_back();
}

}  // namespace wheel
