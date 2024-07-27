#pragma once

#include <wheel/singleton.hpp>

#include <game/layer/layer.hpp>

namespace wheel {

class InventoryLayer final : public Layer, public Singleton<InventoryLayer> {
    friend class Singleton<InventoryLayer>;

public:
    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    void on_render() override;
    bool on_event(const SDL_Event& event) override;

private:
    InventoryLayer() = default;
    ~InventoryLayer() = default;
    InventoryLayer(const InventoryLayer&) = delete;

    SDL_Texture* texture_;
};

}  // namespace wheel


