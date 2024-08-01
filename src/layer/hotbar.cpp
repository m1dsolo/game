#include <game/layer/hotbar.hpp>

#include <game/global.hpp>
#include <game/texture_manager.hpp>

#include <game/component/self.hpp>
#include <game/component/inventory.hpp>

namespace wheel {

HotBarLayer::HotBarLayer() {
    w = slot_count * (slot_w + slot_padding) + slot_padding;
    h = slot_h + slot_padding * 2;
    texture_ = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w, h);
    sdl.set_target(texture_);
    for (int i = 0; i < slot_count; i++) {
        SDL_FRect slot_rect = {(float)slot_padding + i * (slot_w + slot_padding), (float)slot_padding, (float)slot_w, (float)slot_h};
        sdl.draw_rect(&slot_rect, sdl.WHITE);
    }
    sdl.set_target(nullptr);
}

void HotBarLayer::on_attach() {
}

void HotBarLayer::on_detach() {
}

void HotBarLayer::on_update() {

}

void HotBarLayer::on_render() {
    // hotbar
    SDL_FRect dst_rect = {(float)(config_resource.w - w) / 2, (float)(config_resource.h - h), (float)w, (float)h};
    sdl.render(texture_, nullptr, &dst_rect);

    // items
    auto& texture_manager = TextureManager::instance();
    int entity = ecs.get_entities<SelfComponent>()[0];
    auto& inventory = ecs.get_component<InventoryComponent>(entity).inventory;
    int selected_idx = inventory.selected_idx();
    for (int i = 1; i <= slot_count; i++) {
        auto& slot = inventory.slot(i);
        if (slot.empty()) {
            continue;
        }
        const std::string& name = slot.item().data().name;
        SDL_Texture* texture = texture_manager.get_texture(name);

        // draw item
        float tgt_x = (float)(config_resource.w - this->w) / 2 + slot_padding + (i - 1) * (slot_w + slot_padding);
        float tgt_y = (float)(config_resource.h - this->h) + slot_padding;
        SDL_FRect dst_rect = {tgt_x + (float)(slot_w - item_w) / 2, tgt_y + (float)(slot_h - item_h) / 2, (float)item_w, (float)item_h};
        auto [src_w, src_h] = sdl.get_texture_size(texture);
        SDL_FRect src_rect = {0, 0, src_w, src_h};
        sdl.render(texture, &src_rect, &dst_rect);

        // draw count
        int count = slot.count();
        if (count > 1) {
            auto texture = sdl.create_texture(std::to_string(count), sdl.BLACK);
            auto [w, h] = sdl.get_texture_size(texture);
            SDL_FRect src_rect = {0, 0, w, h};
            SDL_FRect dst_rect = {(float)(config_resource.w - this->w) / 2 + slot_padding + (i - 1) * (slot_w + slot_padding) + slot_w - w, (float)(config_resource.h - this->h) + slot_padding + slot_h - h, w, h};
            sdl.render(texture, &src_rect, &dst_rect);
            sdl.destroy(texture);
        }

        // draw progess
        int progress = slot.item().progress();
        if (progress != 100) {
            auto color = sdl.RED;
            color.a = 128;
            float h = (float)slot_h * progress / 100;
            SDL_FRect dst = {tgt_x, tgt_y + slot_h - h, (float)slot_w, h};
            sdl.draw_rect(&dst, color);
        }
    }

    // draw selection
    float x = (float)(config_resource.w - this->w) / 2 + slot_padding + (inventory.selected_idx() - 1) * (slot_w + slot_padding);
    float y = (float)(config_resource.h - this->h) + slot_padding;
    float w = slot_w;
    float h = slot_h;
    SDL_FRect dst = {x, y, w, h};
    sdl.draw_border(&dst, 4, sdl.RED);
}

bool HotBarLayer::on_event(const SDL_Event& event) {
    return false;
}

}  // namespace wheel
