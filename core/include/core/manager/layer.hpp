#pragma once

#include <core/animation.hpp>
#include <core/layer/layer.hpp>

#include <wheel/singleton.hpp>

#include <functional>
#include <unordered_map>
#include <memory>
#include <vector>

#include <rfl.hpp>

namespace core {

class LayerManager : public wheel::Singleton<LayerManager> {
    friend wheel::Singleton<LayerManager>;

public:
    template <typename... Ts> requires (std::derived_from<Ts, Layer> && ...)
    void register_layers() {
        (register_layer<Ts>(), ...);
    }

    template <typename T> requires std::derived_from<T, Layer>
    void register_layer() {
        auto layer = T();
        layer.on_register();
        layer_creators_[layer.name()] = [](){ 
            return std::make_unique<T>(); 
        };
    }

    template <typename... Ts> requires (std::derived_from<Ts, Layer> && ...)
    void push() {
        (push_<Ts>(), ...);
    }

    void pop();

    void update();
    void handle_event(const SDL_Event& event);

private:
    LayerManager();
    LayerManager(const LayerManager&) = delete;

    template <typename T> requires std::derived_from<T, Layer>
    std::unique_ptr<Layer> create_layer_() {
        if (auto iter = layer_creators_.find(T().name()); iter != layer_creators_.end()) {
            return iter->second();
        }
        return std::make_unique<T>();
    }

    template <typename T> requires std::derived_from<T, Layer>
    void push_() {
        if (!layers_.empty()) {
            layers_.back()->on_hide();
        }
        layers_.emplace_back(create_layer_<T>());
        layers_.back()->on_attach();
        layers_.back()->on_show();
        update_systems_();
    }

    void update_systems_();

    std::unordered_map<std::string, std::function<std::unique_ptr<Layer>()>> layer_creators_;
    std::vector<std::unique_ptr<Layer>> layers_;
};

}  // namespace core
