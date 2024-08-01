#include <game/layer/cursor.hpp>

#include <game/global.hpp>

namespace wheel {

CursorLayer::CursorLayer() {
    cursor_texture_ = sdl.create_filled_circle_texture(cursor_radius_, SDL::RED);
    auto [w, h] = sdl.get_texture_size(cursor_texture_);
    cursor_dst_ = {0, 0, w, h};
}

void CursorLayer::on_attach() {
}

void CursorLayer::on_detach() {
}

void CursorLayer::on_update() {
}

void CursorLayer::on_render() {
    sdl.render(cursor_texture_, nullptr, &cursor_dst_);
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
