#include <core/manager/ui.hpp>

namespace core {

UIManager::~UIManager() {
    for (auto& layer : layers_) {
        layer->on_detach();
    }
}

void UIManager::render() {
    for (auto& layer : layers_) {
        layer->on_render();
    }
}

void UIManager::handle_event(const SDL_Event& event) {
    for (auto& layer : layers_) {
        if (layer->on_event(event)) {
            break;
        }
    }
}

void UIManager::pop_front() {
    if (layers_.empty()) {
        return;
    }
    layers_.front()->on_detach();
    layers_.pop_front();
}

void UIManager::pop_back() {
    if (layers_.empty()) {
        return;
    }
    layers_.back()->on_detach();
    layers_.pop_back();
}

}  // namespace core
