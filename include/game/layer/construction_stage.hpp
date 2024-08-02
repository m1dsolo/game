#pragma once

#include <wheel/singleton.hpp>

#include <game/layer/layer.hpp>
#include <game/layer/button.hpp>

namespace wheel {

class ConstructionStageLayer final : public Layer, public Singleton<ConstructionStageLayer> {
    friend class Singleton<ConstructionStageLayer>;

public:
    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    void on_render() override;
    bool on_event(const SDL_Event& event) override;

private:
    ConstructionStageLayer();
    ~ConstructionStageLayer() = default;
    ConstructionStageLayer(const ConstructionStageLayer&) = delete;

    Button start_button_;
};

}  // namespace wheel
