#pragma once

#include <wheel/singleton.hpp>

#include <game/layer/layer.hpp>

#include <game/component/actions.hpp>
#include <game/component/input.hpp>
#include <game/component/position.hpp>
#include <game/component/direction.hpp>

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

    void render_texture();
    void render_health_bar();

    std::unordered_map<std::string, std::shared_ptr<Action>>* action_map_;
    std::unordered_map<SDL_Keycode, std::string>* key_bindings;
    Vector2D<double>* position_;
    Vector2D<double>* aim_direction_;
};

}  // namespace wheel
