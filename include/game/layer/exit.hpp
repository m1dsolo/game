#pragma once

#include <wheel/singleton.hpp>

#include <game/layer/layer.hpp>

namespace wheel {

class ExitLayer final : public Layer, public Singleton<ExitLayer> {
    friend class Singleton<ExitLayer>;

public:
    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    void on_render() override;
    bool on_event(const SDL_Event& event) override;

private:
    ExitLayer();
    ~ExitLayer() = default;
    ExitLayer(const ExitLayer&) = delete;

    float text_field_w_ = 300;
    float text_field_h_ = 200;
    float padding_ = 50;
    float w;
    float h;

    SDL_Texture* texture_;
};

}  // namespace wheel
