#include <game/layer/slots.hpp>

#include <game/global.hpp>
#include <game/texture_manager.hpp>

#include <game/component/self.hpp>
#include <game/component/inventory.hpp>

namespace wheel {

SlotsLayer::SlotsLayer(bool hotbar) {
    m = 10;
    n = hotbar ? 1 : 4;

    w = m * (slot_w + slot_padding) + slot_padding;
    h = n * (slot_h + slot_padding) + slot_padding;
    
    // hotbar: mid bottom
    if (hotbar) {
        main_rect_ = {(float)(config_resource.w - w) / 2, (float)(config_resource.h - h), (float)w, (float)h};
        base_idx = 1;
    // inventory: mid center
    } else {
        main_rect_ = {(float)(config_resource.w - w) / 2, (float)(config_resource.h - h) / 2, (float)w, (float)h};
        base_idx = 11;
    }

    slot_rects_.resize(n * m);
    item_rects_.resize(n * m);

    texture_ = sdl.create_texture(w, h, sdl.BLACK, SDL_TEXTUREACCESS_TARGET);
    sdl.set_target(texture_);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            SDL_FRect slot_rect = {
                (float)j * (slot_w + slot_padding) + slot_padding,
                (float)i * (slot_h + slot_padding) + slot_padding,
                (float)slot_w,
                (float)slot_h
            };
            sdl.draw_rect(&slot_rect, sdl.WHITE);

            slot_rects_[i * m + j] = {main_rect_.x + slot_rect.x, main_rect_.y + slot_rect.y, slot_rect.w, slot_rect.h};
            item_rects_[i * m + j] = {
                main_rect_.x + slot_rect.x + (float)(slot_w - item_w) / 2,
                main_rect_.y + slot_rect.y + (float)(slot_h - item_h) / 2,
                (float)item_w,
                (float)item_h
            };
        }
    }
    sdl.set_target(nullptr);
}

void SlotsLayer::on_attach() {
}

void SlotsLayer::on_detach() {
}

void SlotsLayer::on_update() {
}

// TODO: lazy calculation for performance
void SlotsLayer::on_render() {
    // hotbar
    sdl.render(texture_, nullptr, &main_rect_);

    // items
    auto& texture_manager = TextureManager::instance();
    int entity = ecs.get_entities<SelfComponent>()[0];
    auto& inventory = ecs.get_component<InventoryComponent>(entity).inventory;
    int selected_idx = inventory.selected_idx();
    for (int i = 0; i < n * m; i++) {
        auto& slot = inventory.slot(base_idx + i);
        if (slot.empty()) {
            continue;
        }
        const std::string& name = slot.item().data().name;
        SDL_Texture* texture = texture_manager.get_texture(name);

        // draw item
        auto [src_w, src_h] = sdl.get_texture_size(texture);
        SDL_FRect src_rect = {0, 0, src_w, src_h};
        sdl.render(texture, &src_rect, &item_rects_[i]);

        // draw count
        int count = slot.count();
        if (count > 1) {
            auto texture = sdl.create_texture(std::to_string(count), sdl.BLACK);
            auto [w, h] = sdl.get_texture_size(texture);
            SDL_FRect src_rect = {0, 0, w, h};
            SDL_FRect dst_rect = {slot_rects_[i].x + slot_w - w, slot_rects_[i].y + slot_h - h, w, h};
            sdl.render(texture, &src_rect, &dst_rect);
            sdl.destroy(texture);
        }

        // draw progess
        int progress = slot.item().progress();
        if (progress != 100) {
            auto color = sdl.RED;
            color.a = 128;
            float h = (float)slot_h * progress / 100;
            SDL_FRect dst = {slot_rects_[i].x, slot_rects_[i].y + slot_h - h, (float)slot_w, h};
            sdl.draw_rect(&dst, color);
        }
    }

    // draw selection
    int idx = inventory.selected_idx() - base_idx;
    if (idx >= 0 && idx < n * m) {
        sdl.draw_border(&slot_rects_[idx], 4, sdl.RED);
    }
}

bool SlotsLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_MOUSE_MOTION: {
            float x = event.motion.x, y = event.motion.y;
            mouse_target_idx = -1;
            for (int i = 0; i < n * m; i++) {
                auto [x, y, w, h] = slot_rects_[i];
                if (event.motion.x >= x && event.motion.x <= x + w && event.motion.y >= y && event.motion.y <= y + h) {
                    mouse_target_idx = i;
                    break;
                }
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            if (mouse_target_idx != -1) {
                int entity = ecs.get_entities<SelfComponent>()[0];
                auto& inventory = ecs.get_component<InventoryComponent>(entity).inventory;
                inventory.select(mouse_target_idx + base_idx);
                return true;
            }
            break;
        }
    }
    return false;
}

}  // namespace wheel
