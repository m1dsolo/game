#include <core/manager/sprite.hpp>
#include <core/global.hpp>
#include <core/config/sprite.hpp>
#include <core/manager/texture.hpp>
#include <core/manager/item.hpp>
#include <core/resource/context.hpp>

#include <sdl/sdl.hpp>
#include <rfl/json.hpp>

#include <filesystem>
#include <iostream>

namespace core {

SpriteManager::SpriteManager() {
    // set("", {sdl::SDL::create_texture(48, 48, sdl::SDL::Color::Purple)});
    set("", {nullptr});
    set("main_menu_layout", {sdl::SDL::create_texture(1, 1, sdl::SDL::Color::Gray)});
    set("pause_menu_layout", {sdl::SDL::create_texture(1, 1, sdl::SDL::Color::Gray)});
    set("button_normal", {sdl::SDL::create_texture(1, 1, { 230.f / 255.f, 230.f / 255.f, 230.f / 255.f, 1.f })});
    set("button_hovered", {sdl::SDL::create_texture(1, 1, { 150.f / 255.f, 190.f / 255.f, 250.f / 255.f, 1.f })});
    set("button_pressed", {sdl::SDL::create_texture(1, 1, { 200.f / 255.f, 160.f / 255.f, 250.f / 255.f, 1.f })});

    init_map_sprites_();
    init_hp_bar_sprites_();
    init_slot_item_num_sprites_();
    init_aura_sprites_();
    init_range_attack_sprites_();
    init_item_info_sprites_();

    for (const auto& entry : std::filesystem::recursive_directory_iterator("assets/sprite")) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            std::ifstream file(entry.path());
            if (file.is_open()) {
                std::cout << "[begin load sprite...]" << entry.path() << std::endl;
                const auto [path, size, sprites, pixel] = rfl::json::read<SpritesConfig>(file).value();
                auto texture = TextureManager::instance().get(std::filesystem::path("sprite_sheet") / path);
                if (!pixel.value_or(true)) {
                    sdl::SDL::set_texture_scalemode(texture, SDL_SCALEMODE_LINEAR);
                }
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
        auto texture = sdl::SDL::create_texture(std::to_string(i), 16.f, sdl::SDL::Color::Cyan);
        auto [w, h] = sdl::SDL::get_texture_size(texture);
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

void SpriteManager::init_range_attack_sprites_() {
    set("range_attack", Sprite{
        sdl::SDL::create_circle_texture(500.f, sdl::SDL::Color::Red, 5.f),
        {0.f, 0.f, 1000.f, 1000.f}
    });
}

void SpriteManager::init_item_info_sprites_() {
    static const std::unordered_map<Item::Rarity, SDL_FColor> rarity2color_ {
        {Item::Rarity::common, sdl::SDL::Color::White},
        {Item::Rarity::uncommon, sdl::SDL::Color::Green},
        {Item::Rarity::rare, sdl::SDL::Color::Blue},
        {Item::Rarity::epic, sdl::SDL::Color::Purple},
        {Item::Rarity::legendary, sdl::SDL::Color::Orange}
    };

    std::cout << "item_info: " << wheel::ID("item_info") << std::endl;
    for (const auto& [name, sprite, rarity, type, weight, description, components, _] : ItemManager::instance().item_configs()) {
        if (name == "") {
            continue;
        }

        auto texture = sdl::SDL::create_texture(300, 512, sdl::SDL::Color::Gray, SDL_TEXTUREACCESS_TARGET);
        sdl::SDL::RenderTargetGuard guard(texture);
        SDL_FRect dst = {0.f, 0.f, 300.f, 512.f};
        sdl::SDL::render_rect(&dst, sdl::SDL::Color::Black, 8.f);
        float y = 16.f;
        sdl::SDL::render_text(texture, 16.f, y, name, 24.f, rarity2color_.at(rarity));
        sdl::SDL::render_text(texture, 16.f, y += 24.f, rfl::enum_to_string(type), 16.f, sdl::SDL::Color::Pink);
        sdl::SDL::render_text(texture, 16.f, y += 24.f, std::format("weight: {}", weight), 16.f, sdl::SDL::Color::Black);
        for (const auto& line : description) {
            sdl::SDL::render_text(texture, 16.f, y += 24.f, line, 16.f, sdl::SDL::Color::White);
        }

        if (components.range_attack.has_value()) {
            const auto& range_attack = components.range_attack.value();
            sdl::SDL::render_text(texture, 32.f, y += 24.f, std::format("Damage: {}", range_attack.damage), 16.f, sdl::SDL::Color::Red);
            sdl::SDL::render_text(texture, 32.f, y += 24.f, std::format("Attack speed: {:.2f}s", range_attack.interval / 1000000.f), 16.f, sdl::SDL::Color::White);
        }

        set(wheel::ID("item_info") ^ (wheel::ID(name) + static_cast<int>(rarity)), {texture});
    }
}

}  // namespace core
