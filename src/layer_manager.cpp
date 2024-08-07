#include <game/layer_manager.hpp>

#include <game/layer/card.hpp>
#include <game/layer/slots.hpp>

namespace wheel {

LayerManager::LayerManager() {
    layer_map_["player_cards"] = std::make_shared<CardLayer>(true);
    layer_map_["enemy_cards"] = std::make_shared<CardLayer>(false);
    layer_map_["hotbar"] = std::make_shared<SlotsLayer>(true);
    layer_map_["inventory"] = std::make_shared<SlotsLayer>(false);
}

Layer* LayerManager::get(const std::string& name) {
    return layer_map_[name].get();
}

}  // namespace wheel
