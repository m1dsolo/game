#pragma once

#include <core/manager/layer.hpp>
#include <core/layer/layer.hpp>

#include <wheel/singleton.hpp>

#include <deque>
#include <memory>

namespace core {

class UIManager : public wheel::Singleton<UIManager> {
    friend wheel::Singleton<UIManager>;

public:
    void update();
    void handle_event(const SDL_Event& event);

    template <typename... Ts> requires (std::derived_from<Ts, Layer> && ...)
    void push_front() {
        (push_front_<Ts>(), ...);
    }

    template <typename... Ts> requires (std::derived_from<Ts, Layer> && ...)
    void push_back() {
        (push_back_<Ts>(), ...);
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
    template <typename T> requires std::derived_from<T, Layer>
    void push_front_() {
        layers_.emplace_front(LayerManager::instance().create_layer<T>());
        layers_.front()->on_attach();
    }

    template <typename T> requires std::derived_from<T, Layer>
    void push_back_() {
        layers_.emplace_back(LayerManager::instance().create_layer<T>());
        layers_.back()->on_attach();
    }

    UIManager() = default;
    ~UIManager();
    UIManager(const UIManager&) = delete;

    std::deque<std::unique_ptr<Layer>> layers_;
};

}  // namespace core
