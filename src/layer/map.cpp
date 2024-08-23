#include <game/layer/map.hpp>

#include <game/global.hpp>

#include <game/component/self.hpp>
#include <game/component/inventory.hpp>

namespace wheel {

void MapLayer::on_attach() {
    Entity entity = ecs.get_entities<SelfComponent>()[0];
    inventory_ = &ecs.get_component<InventoryComponent>(entity).inventory;
}

void MapLayer::on_detach() {
}

void MapLayer::on_update() {
}

void MapLayer::on_render() {
    // render map
    auto pos = camera.left_top();
    auto& [screen_w, screen_h] = camera.size();
    auto [real_x0, real_y0, real_x1, real_y1] = map.real_rect();
    SDL_FRect src = {(float)pos.x + real_x0, (float)pos.y + real_y0, (float)screen_w, (float)screen_h};
    sdl.render(map.texture(), &src, nullptr);

    // render selection
    auto& item = inventory_->selected_slot().item();
    if (!item.empty() && item.data().show_selected_tile) {
        if (tile_pos_.first != -1 && tile_pos_.second != -1) {
            auto pos = map.idx2pos(tile_pos_.first, tile_pos_.second);
            float tile_size = map.TILE_SIZE;
            pos = camera.world2screen(pos);
            SDL_FRect dst = {pos.x - tile_size / 2, pos.y - tile_size / 2, tile_size, tile_size};
            sdl.draw_border(&dst, 4, sdl.GREEN);
        }
    }
}

bool MapLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_MOUSE_MOTION: {
            float x = event.motion.x, y = event.motion.y;
            auto [world_x, world_y] = camera.screen2world({x, y});
            tile_pos_ = map.pos2idx({world_x, world_y});
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
