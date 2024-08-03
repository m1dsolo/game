#pragma once

#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>

#include <game/layer/layer.hpp>
#include <game/inventory.hpp>

namespace wheel {

class CursorLayer final : public Layer, public Singleton<CursorLayer> {
    friend class Singleton<CursorLayer>;

public:
    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    void on_render() override;
    bool on_event(const SDL_Event& event) override;

private:
    CursorLayer();
    ~CursorLayer() = default;
    CursorLayer(const CursorLayer&) = delete;

    SDL_FRect cursor_dst_ {0., 0., 0., 0.};
    Inventory* inventory_;
};

}  // namespace wheel
