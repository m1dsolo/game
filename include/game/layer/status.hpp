#pragma once

#include <wheel/singleton.hpp>

#include <game/layer/layer.hpp>

namespace wheel {

class StatusLayer final : public Layer, public Singleton<StatusLayer> {
    friend class Singleton<StatusLayer>;

public:
    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    void on_render() override;
    bool on_event(const SDL_Event& event) override;

private:
    StatusLayer() = default;
    ~StatusLayer() = default;
    StatusLayer(const StatusLayer&) = delete;

    int width;
    int height;
};

}  // namespace wheel
