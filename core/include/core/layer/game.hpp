#pragma once

#include <core/layer/layer.hpp>

#include <ecs/entity.hpp>

namespace core {

class GameLayer : public Layer {
public:
    void on_attach() override;
    // void on_detach() override;
    void on_render() override;
    bool on_event(const SDL_Event& event) override;

private:
    wheel::Entity player_entity_;

    // 0. map
    // 1. player, enemy, range, aura
    // 2. hp_bar, bullet
    static const unsigned int MAX_LAYER = 2;

    bool is_move_up_{false};
    bool is_move_down_{false};
    bool is_move_left_{false};
    bool is_move_right_{false};
};

}  // namespace core
