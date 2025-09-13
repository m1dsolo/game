#pragma once

#include <core/animation.hpp>
#include <core/layer/layer.hpp>

#include <wheel/singleton.hpp>

#include <deque>
#include <memory>

namespace core {

class UIManager : public wheel::Singleton<UIManager> {
    friend wheel::Singleton<UIManager>;

public:
    void render();
    void handle_event(const SDL_Event& event);

    template <typename... Ts> requires (std::derived_from<Ts, Layer> && ...)
    void push() {
        (push_back<Ts>(), ...);
    }

    template <typename T> requires std::derived_from<T, Layer>
    void push_front() {
        layers_.emplace_front(std::make_unique<T>());
        layers_.front()->on_attach();
    }

    template <typename T> requires std::derived_from<T, Layer>
    void push_back() {
        layers_.emplace_back(std::make_unique<T>());
        layers_.back()->on_attach();
    }

    template <typename T> requires std::derived_from<T, Layer>
    void del() {
        for (auto iter = layers_.begin(); iter != layers_.end(); iter++) {
            if (dynamic_cast<T*>((*iter).get())) {
                (*iter)->on_detach();
                layers_.erase(iter);
                return;
            }
        }
    }

    void pop_front();
    void pop_back();

private:
    UIManager() = default;
    ~UIManager();
    UIManager(const UIManager&) = delete;

    std::deque<std::unique_ptr<Layer>> layers_;
};

}  // namespace core
