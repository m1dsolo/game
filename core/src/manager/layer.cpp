#include <core/manager/layer.hpp>
#include <core/layer/global.hpp>
#include <core/layer/main_menu.hpp>
#include <core/layer/game.hpp>

namespace core {

LayerManager::LayerManager() {
    register_layers<
        GlobalLayer,
        MainMenuLayer,
        GameLayer
    >();
}

}  // namespace core
