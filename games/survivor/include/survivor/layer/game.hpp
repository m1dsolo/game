#pragma once

#include <core/layer/game.hpp>

#include <ecs/entity.hpp>

namespace survivor {

class GameLayer : public core::GameLayer {
public:
    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    // bool on_event(const SDL_Event& event) override;

private:
    wheel::Entity player_entity_;
    wheel::Entity text_entity_;
};

}  // namespace survivor
