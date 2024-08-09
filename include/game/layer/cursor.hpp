#pragma once

#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>

#include <game/layer/layer.hpp>
#include <game/inventory.hpp>

namespace wheel {

class CursorLayer final : public Layer {
public:
    CursorLayer();
    ~CursorLayer() = default;
    CursorLayer(const CursorLayer&) = delete;

    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    void on_render() override;
    bool on_event(const SDL_Event& event) override;

    void set_texture(SDL_Texture* texture);

private:
    SDL_Texture* texture_ = nullptr;
    SDL_FRect cursor_dst_ {0., 0., 0., 0.};
    Inventory* inventory_;
};

}  // namespace wheel
