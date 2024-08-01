#pragma once

#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>

#include <game/layer/layer.hpp>

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

    SDL_Texture* cursor_texture_ = nullptr;
    SDL_FRect cursor_dst_;
    static const int cursor_radius_ = 5;
};

}  // namespace wheel
