#include <game/layer/map.hpp>

#include <game/global.hpp>

namespace wheel {

void MapLayer::on_attach() {
}

void MapLayer::on_detach() {
}

void MapLayer::on_update() {
}

void MapLayer::on_render() {
    // render map
    float w = map_resource.tilemap[0].size() * 48;
    float h = map_resource.tilemap.size() * 48;
    float x = (config_resource.w - w) / 2;
    float y = (config_resource.h - h) / 2;
    SDL_FRect dst_frect = {x, y, w, h};
    sdl.render(map_resource.texture, nullptr, &dst_frect);

    // render selection
    if (tile_pos_.first != -1 && tile_pos_.second != -1) {
        Vector2D<double> pos = map_resource.idx2pos(tile_pos_.first, tile_pos_.second);
        SDL_FRect dst = {(float)pos.x - 24, (float)pos.y - 24, 48, 48};
        sdl.draw_boarder(&dst, 4, sdl.GREEN);
    }
}

bool MapLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_MOUSE_MOTION: {
            float x = event.motion.x, y = event.motion.y;
            tile_pos_ = map_resource.pos2idx({x, y});
            return true;
            
        }
        // case SDL_EVENT_KEY_DOWN: {
        //     tile_pos_ = {-1, -1};
        //     break;
        // }
    }

    return false;
}

}  // namespace wheel
