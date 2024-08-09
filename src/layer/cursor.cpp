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
    auto texture = texture_;
    if (!texture) {
        texture = slot.empty() ? TextureManager::instance().get_texture("circle_cursor") : slot.item().get_cursor_texture();
    }
    auto [w, h] = sdl.get_texture_size(texture);
    cursor_dst_.w = w;
    cursor_dst_.h = h;
    sdl.render(texture, nullptr, &cursor_dst_);
}

bool CursorLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_MOUSE_MOTION: {
            float mouse_x = event.motion.x, mouse_y = event.motion.y;
            cursor_dst_.x = mouse_x - cursor_dst_.w / 2;
            cursor_dst_.y = mouse_y - cursor_dst_.h / 2;
            break;
        }
    }
    return false;
}

void CursorLayer::set_texture(SDL_Texture* texture) {
    texture_ = texture;
}

}  // namespace wheel
