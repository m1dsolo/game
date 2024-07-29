#pragma once

#include <utility>

#include <wheel/singleton.hpp>

#include <game/layer/layer.hpp>

namespace wheel {

class MapLayer final : public Layer, public Singleton<MapLayer> {
    friend class Singleton<MapLayer>;

public:
    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    void on_render() override;
    bool on_event(const SDL_Event& event) override;

private:
    MapLayer() = default;
    ~MapLayer() = default;
    MapLayer(const MapLayer&) = delete;

    std::pair<int, int> tile_pos_;
};

}  // namespace wheel

