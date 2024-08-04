#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include <wheel/singleton.hpp>

#include <game/layer/layer.hpp>

namespace wheel {

class LayerManager : public Singleton<LayerManager> {
    friend class Singleton<LayerManager>;

public:
    Layer* get(const std::string& name);

private:
    LayerManager();
    ~LayerManager() = default;
    LayerManager(const LayerManager&) = delete;

    std::unordered_map<std::string, std::shared_ptr<Layer>> layer_map_;
};

}  // namespace wheel
