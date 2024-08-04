#pragma once

#include <concepts>
#include <deque>

#include <wheel/singleton.hpp>

#include <game/layer/layer.hpp>
#include <game/layer/cursor.hpp>

namespace wheel {

// TODO: layers memory management, no singleton layer
class UI final : public Singleton<UI> {
    friend class Singleton<UI>;

public:
    void update();
    void render();
    void handle_event(const SDL_Event& event);

    template <typename T> requires std::derived_from<T, Layer>
    void push_front() {
        T& t = T::instance();
        t.on_attach();
        layers_.push_front(&t);
    }

    template <typename T> requires std::derived_from<T, Layer>
    void push_back() {
        T& t = T::instance();
        t.on_attach();
        layers_.push_back(&t);
    }

    void push_back(Layer* layer) {
        layer->on_attach();
        layers_.push_back(layer);
    }

    template <typename T> requires std::derived_from<T, Layer>
    void del() {
        for (auto it = layers_.begin(); it != layers_.end(); ++it) {
            if (dynamic_cast<T*>(*it)) {
                (*it)->on_detach();
                layers_.erase(it);
                break;
            }
        }
    }

    void pop_front();
    void pop_back();

    std::deque<Layer*>& layers() { return layers_; }

private:
    UI() : cursor_layer_(&CursorLayer::instance()) {}
    ~UI();
    UI(const UI&) = delete;

    std::deque<Layer*> layers_;
    Layer* cursor_layer_;
};

}  // namespace wheel
