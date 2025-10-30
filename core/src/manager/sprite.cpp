#include <core/manager/sprite.hpp>
#include <core/global.hpp>
#include <core/config/sprite.hpp>
#include <core/manager/texture.hpp>
#include <core/resource/context.hpp>
#include <core/resource/inventory.hpp>

#include <sdl/sdl.hpp>
#include <rfl/json.hpp>

#include <filesystem>
#include <iostream>

namespace core {

SpriteManager::SpriteManager() {
    set("", {sdl::SDL::create_texture(48, 48, sdl::SDL::Color::Purple)});
    set("main_menu_layout", {sdl::SDL::create_texture(1, 1, sdl::SDL::Color::Gray)});
    set("pause_menu_layout", {sdl::SDL::create_texture(1, 1, sdl::SDL::Color::Gray)});
    set("button_normal", {sdl::SDL::create_texture(1, 1, { 230.f / 255.f, 230.f / 255.f, 230.f / 255.f, 1.f })});
    set("button_hovered", {sdl::SDL::create_texture(1, 1, { 150.f / 255.f, 190.f / 255.f, 250.f / 255.f, 1.f })});
    set("button_pressed", {sdl::SDL::create_texture(1, 1, { 200.f / 255.f, 160.f / 255.f, 250.f / 255.f, 1.f })});

    init_map_sprites_();
    init_hp_bar_sprites_();
    init_slot_item_num_sprites_();
    init_aura_sprites_();
    init_auto_shoot_sprites_();
    init_inventory_sprites_();

    for (const auto& entry : std::filesystem::recursive_directory_iterator("assets/sprite")) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            std::ifstream file(entry.path());
            if (file.is_open()) {
                std::cout << "[begin load sprite...]" << entry.path() << std::endl;
                const auto [path, size, sprites] = rfl::json::read<SpritesConfig>(file).value();
                auto texture = TextureManager::instance().get(std::filesystem::path("sprite_sheet") / path);
                for (const auto& [name, pos] : sprites) {
                    set(name, {texture, {
                        static_cast<float>(pos.first),
                        static_cast<float>(pos.second),
                        static_cast<float>(size.first),
                        static_cast<float>(size.second)
                    }});
                }
                file.close();
                std::cout << "[end load sprite...]" << std::endl;
            }
        }
    }
}

const Sprite& SpriteManager::set(wheel::ID id, const Sprite& sprite) {
    return id2sprites_[id] = sprite;
}

const Sprite& SpriteManager::get(wheel::ID id) const {
    if (id2sprites_.find(id) == id2sprites_.end()) {
        std::cout << "not found sprite: " << id << "!!!!!" << std::endl;
        return id2sprites_.at("");
    }
    // std::cout << "sprite_get: " << name << std::endl;
    return id2sprites_.at(id);
}

bool SpriteManager::has(wheel::ID id) const {
    return id2sprites_.find(id) != id2sprites_.end();
}

void SpriteManager::del(wheel::ID id) {
    if (auto iter = id2sprites_.find(id); iter != id2sprites_.end()) {
        sdl::SDL::destroy(iter->second.texture);
        id2sprites_.erase(iter);
    }
}

void SpriteManager::init_map_sprites_() {
    const auto& context = ecs.get_resource<ContextResource>();
    float map_width = context.map_width;
    float map_height = context.map_height;
    set("map", Sprite{
        sdl::SDL::create_texture(map_width, map_height, SDL_FColor{204.f / 255.f, 1.f, 153.f / 255.f, 1.f}),
        {0.f, 0.f, map_width, map_height}
    });
}

void SpriteManager::init_hp_bar_sprites_() {
    // init hp bar sprites
    for (int i = 1; i <= 47; i++) {
        auto texture = sdl::SDL::create_texture(50, 10, sdl::SDL::Color::Black);
        sdl::SDL::RenderTargetGuard guard{texture};
        auto dst = SDL_FRect{1.f, 1.f, 48.f, 8.f};
        sdl::SDL::render_filled_rect(&dst, sdl::SDL::Color::Red);
        dst = SDL_FRect{1.f, 1.f, static_cast<float>(i), 8.f};
        sdl::SDL::render_filled_rect(&dst, sdl::SDL::Color::Green);
        set(std::format("hp_bar{}", i), {
            texture,
            {0.f, 0.f, 50.f, 10.f}
        });
    }
    // hidden hp bar when hp is empty or full
    set("hp_bar0", {nullptr, {0.f, 0.f, 50.f, 10.f}});
    set("hp_bar48", {nullptr, {0.f, 0.f, 50.f, 10.f}});
}

void SpriteManager::init_slot_item_num_sprites_() {
    for (int i = 1; i <= 99; i++) {
        auto texture = sdl::SDL::create_texture(std::to_string(i), 16.f, sdl::SDL::Color::Black);
        auto [w, h] = sdl::SDL::get_texture_size(texture);
        sdl::SDL::RenderTargetGuard guard{texture};
        set(std::to_string(i), {
            texture,
            {0.f, 0.f, w, h}
        });
    }
}

void SpriteManager::init_aura_sprites_() {
    set("damage_aura", Sprite{
        sdl::SDL::create_filled_circle_texture(500.f, sdl::SDL::Color::Pink),
        {0.f, 0.f, 1000.f, 1000.f}
    });
}

void SpriteManager::init_auto_shoot_sprites_() {
    set("auto_shoot", Sprite{
        sdl::SDL::create_circle_texture(500.f, sdl::SDL::Color::Red, 5.f),
        {0.f, 0.f, 1000.f, 1000.f}
    });
}

void SpriteManager::init_inventory_sprites_() {
    auto& resource = ecs.get_resource<InventoryResource>();
    const auto& slot_nums = resource.slot_nums;
    auto& slot_rects = resource.slot_rects;
    slot_rects.resize(slot_nums[0], std::vector<SDL_FRect>(slot_nums[1]));

    static constexpr float slot_sizes[2] = { 64.f, 64.f };
    static constexpr float slot_spacing_sizes[2] = { 5.f, 5.f };
    static constexpr float inventory_sizes[2] = {
        slot_nums[0] * (slot_sizes[0] + slot_spacing_sizes[0]) + slot_spacing_sizes[0],
        slot_nums[1] * (slot_sizes[1] + slot_spacing_sizes[1]) + slot_spacing_sizes[1]
    };

    // create inventory background layer sprite
    {
        auto texture = sdl::SDL::create_texture(
            inventory_sizes[0], inventory_sizes[1],
            sdl::SDL::Color::White,
            SDL_TEXTUREACCESS_TARGET
        );
        sdl::SDL::RenderTargetGuard guard(texture);
        for (int i = 0; i < slot_nums[0]; ++i) {
            for (int j = 0; j < slot_nums[1]; ++j) {
                slot_rects[i][j] = {
                    i * (slot_sizes[0] + slot_spacing_sizes[0]) + slot_spacing_sizes[0],
                    j * (slot_sizes[1] + slot_spacing_sizes[1]) + slot_spacing_sizes[1],
                    slot_sizes[0],
                    slot_sizes[1]
                };
                sdl::SDL::render_rect(&slot_rects[i][j], sdl::SDL::Color::Black);
            }
        }
        set("inventory_background_layer", Sprite{texture});
    }

    // create inventory items layer sprite
    {
        auto texture = sdl::SDL::create_texture(
            inventory_sizes[0], inventory_sizes[1],
            sdl::SDL::Color::Transparent,
            SDL_TEXTUREACCESS_TARGET
        );
        set("inventory_items_layer", Sprite{texture});
    }

    // create inventory rarities layer sprite
    {
        auto texture = sdl::SDL::create_texture(
            inventory_sizes[0], inventory_sizes[1],
            sdl::SDL::Color::Transparent,
            SDL_TEXTUREACCESS_TARGET
        );
        set("inventory_rarities_layer", Sprite{texture});
    }

    // create inventory items count layer sprite
    {
        auto texture = sdl::SDL::create_texture(
            inventory_sizes[0], inventory_sizes[1],
            sdl::SDL::Color::Transparent,
            SDL_TEXTUREACCESS_TARGET
        );
        set("inventory_items_count_layer", Sprite{texture});
    }
}

}  // namespace core
