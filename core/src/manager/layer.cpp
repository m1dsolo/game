#include <core/manager/layer.hpp>
#include <core/manager/system.hpp>
#include <core/layer/global.hpp>
#include <core/layer/main_menu.hpp>
#include <core/layer/pause_menu.hpp>
#include <core/layer/game.hpp>
#include <core/layer/inventory.hpp>

namespace core {

LayerManager::LayerManager() {
    register_layers<
        GlobalLayer,
        MainMenuLayer,
        PauseMenuLayer,
        GameLayer,
        InventoryLayer
    >();
}

void LayerManager::pop() {
    if (layers_.empty()) {
        return;
    }
    layers_.back()->on_hide();
    layers_.back()->on_detach();
    layers_.pop_back();
    if (!layers_.empty()) {
        layers_.back()->on_show();
    }
    update_systems_();
}

void LayerManager::update() {
    for (const auto& layer : layers_) {
        layer->on_update();
    }
}

void LayerManager::handle_event(const SDL_Event& event) {
    for (auto iter = layers_.rbegin(); iter != layers_.rend(); iter++) {
        if ((*iter)->on_event(event)) {
            break;
        }
    }
}

void LayerManager::update_systems_() {
    if (!layers_.empty() && layers_.back()->name() == "GameLayer") {
        SystemManager::instance().resume_game_systems();
    } else {
        SystemManager::instance().pause_game_systems();
    }
}

}  // namespace core
