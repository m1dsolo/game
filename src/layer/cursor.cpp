#include <game/layer/cursor.hpp>

#include <game/global.hpp>
#include <game/texture_manager.hpp>
#include <game/component/self.hpp>
#include <game/component/inventory.hpp>

namespace wheel {

CursorLayer::CursorLayer() {
    Entity entity = ecs.get_entities<SelfComponent>()[0];
    inventory_ = &ecs.get_component<InventoryComponent>(entity).inventory;
}

void CursorLayer::on_attach() {
}

void CursorLayer::on_detach() {
}

void CursorLayer::on_update() {
}

void CursorLayer::on_render() {
    auto& slot = inventory_->selected_slot();
    auto texture = slot.empty() ? TextureManager::instance().get_texture("circle_cursor") : slot.item().get_cursor_texture();
    auto [w, h] = sdl.get_texture_size(texture);
    cursor_dst_.w = w;
    cursor_dst_.h = h;
    sdl.render(texture, nullptr, &cursor_dst_);
}

bool CursorLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_MOUSE_MOTION: {
            cursor_dst_.x = event.motion.x;
            cursor_dst_.y = event.motion.y;
            break;
        }
    }
    return false;
}

}  // namespace wheel
