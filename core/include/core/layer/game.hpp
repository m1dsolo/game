#pragma once

#include <core/layer/layer.hpp>

namespace core {

class GameLayer : public Layer {
public:
    GameLayer() {}

    void on_attach() override;
    void on_detach() override;
    void on_show() override;
    void on_hide() override;
    bool on_event(const SDL_Event& event) override;

    inline static bool new_game_requested = false;
};

}  // namespace core
