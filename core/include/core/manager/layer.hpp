#pragma once

#include <core/animation.hpp>
#include <core/layer/layer.hpp>

#include <wheel/singleton.hpp>

#include <functional>
#include <unordered_map>
#include <memory>

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
        layer_creators_[T().name()] = [](){ 
            return std::make_unique<T>(); 
        };
    }

    template <typename T> requires std::derived_from<T, Layer>
    std::unique_ptr<Layer> create_layer() {
        auto iter = layer_creators_.find(T().name());
        if (iter != layer_creators_.end()) {
            return iter->second();
        }
        return std::make_unique<T>();
    }

private:
    LayerManager();
    LayerManager(const LayerManager&) = delete;

    std::unordered_map<std::string, std::function<std::unique_ptr<Layer>()>> layer_creators_;
};

}  // namespace core
