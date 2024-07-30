#include <game/layer/map.hpp>

#include <game/global.hpp>

#include <game/component/self.hpp>
#include <game/component/inventory.hpp>

namespace wheel {

void MapLayer::on_attach() {
    Entity entity = ecs.get_entities<SelfComponent>()[0];
    inventory_ = &ecs.get_component<InventoryComponent>(entity).inventory;

    TILE_SIZE = map_resource.TILE_SIZE;

    float w = map_resource.tilemap[0].size() * TILE_SIZE;
    float h = map_resource.tilemap.size() * TILE_SIZE;
    float x = (config_resource.w - w) / 2;
    float y = (config_resource.h - h) / 2;
    map_dst_ = {x, y, w, h};
}

void MapLayer::on_detach() {
}

void MapLayer::on_update() {
}

void MapLayer::on_render() {
    // render map
    sdl.render(map_resource.texture, nullptr, &map_dst_);

    // render selection
    auto& item = inventory_->selected_slot().item();
    if (!item.empty() && item.data().show_selected_tile) {
        if (tile_pos_.first != -1 && tile_pos_.second != -1) {
            Vector2D<double> pos = map_resource.idx2pos(tile_pos_.first, tile_pos_.second);
            pos.x += map_dst_.x;
            pos.y += map_dst_.y;
            SDL_FRect dst = {(float)pos.x - TILE_SIZE / 2, (float)pos.y - TILE_SIZE / 2, TILE_SIZE, TILE_SIZE};
            sdl.draw_boarder(&dst, 4, sdl.GREEN);
        }
    }
}

bool MapLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_MOUSE_MOTION: {
            float x = event.motion.x, y = event.motion.y;
            tile_pos_ = map_resource.pos2idx({x - map_dst_.x, y - map_dst_.y});
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
