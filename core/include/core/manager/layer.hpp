#pragma once

#include <core/animation.hpp>
#include <core/layer/layer.hpp>
#include <core/util/utils.hpp>

#include <wheel/singleton.hpp>
#include <wheel/id.hpp>

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
        layer_creators_[Utils::get_type_name<T>()] = [](){ 
            return std::make_unique<T>(); 
        };
    }

    void push(const std::string& layer_name);
    void pop();

    void update();
    void handle_event(const SDL_Event& event);

private:
    LayerManager();
    LayerManager(const LayerManager&) = delete;

    std::unordered_map<std::string, std::function<std::unique_ptr<Layer>()>> layer_creators_;
    std::vector<std::unique_ptr<Layer>> layers_;
};

}  // namespace core
