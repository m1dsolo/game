#pragma once

#include <core/layer/menu.hpp>

namespace core {

class MainMenuLayer : public MenuLayer {
public:
    MainMenuLayer() : MenuLayer("MainMenuLayer") {}

    void on_attach() override;
    void on_detach() override;
    // void on_update() override;
};

}  // namespace core
