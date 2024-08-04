#include <game/layer_manager.hpp>

#include <game/layer/card.hpp>

namespace wheel {

LayerManager::LayerManager() {
    layer_map_["player_cards"] = std::make_shared<CardLayer>(true);
    layer_map_["enemy_cards"] = std::make_shared<CardLayer>(false);
}

Layer* LayerManager::get(const std::string& name) {
    return layer_map_[name].get();
}

}  // namespace wheel
