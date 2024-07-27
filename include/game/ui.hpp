#pragma once

#include <concepts>
#include <deque>

#include <wheel/singleton.hpp>

#include <game/layer/layer.hpp>

namespace wheel {

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

    void pop_front();
    void pop_back();

private:
    UI() = default;
    ~UI() = default;
    UI(const UI&) = delete;

    std::deque<Layer*> layers_;
};

}  // namespace wheel
