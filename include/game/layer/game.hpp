#pragma once

#include <wheel/singleton.hpp>
#include <wheel/ecs.hpp>

#include <game/layer/layer.hpp>

namespace wheel {

class GameLayer final : public Layer, public Singleton<GameLayer> {
    friend class Singleton<GameLayer>;

public:
    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    void on_render() override;
    bool on_event(const SDL_Event& event) override;

private:
    GameLayer() = default;
    ~GameLayer() = default;
    GameLayer(const GameLayer&) = delete;

    Entity self_entity_;
};

}  // namespace wheel
