#include <core/layer/inventory.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/sprite.hpp>
#include <core/manager/item.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/render.hpp>
#include <core/component/item.hpp>
#include <core/component/inventory.hpp>
#include <core/tag/input.hpp>
#include <core/tag/render.hpp>
#include <core/tag/layer.hpp>
#include <core/resource/context.hpp>
#include <core/entity_event/remove_layer.hpp>
#include <core/entity_event/remove_entity.hpp>

#include <rfl/enums.hpp>

namespace core {

void InventoryLayer::on_register() {
    auto& entity_manager = EntityManager::instance();
    const auto& context = ecs.get_resource<ContextResource>();

    slot_rects_.resize(slot_nums_[0], std::vector<SDL_FRect>(slot_nums_[1]));

    // create inventory entity
    wheel::Entity inventory;
    {
        auto texture = sdl::SDL::create_texture(
            inventory_sizes_[0], inventory_sizes_[1],
            sdl::SDL::Color::White,
            SDL_TEXTUREACCESS_TARGET
        );
        sdl::SDL::RenderTargetGuard guard(texture);
        for (int i = 0; i < slot_nums_[0]; ++i) {
            for (int j = 0; j < slot_nums_[1]; ++j) {
                slot_rects_[i][j] = {
                    i * (slot_sizes_[0] + slot_padding_sizes_[0]) + slot_padding_sizes_[0],
                    j * (slot_sizes_[1] + slot_padding_sizes_[1]) + slot_padding_sizes_[1],
                    slot_sizes_[0],
                    slot_sizes_[1]
                };
                sdl::SDL::render_rect(&slot_rects_[i][j], sdl::SDL::Color::Black);
            }
        }
        SpriteManager::instance().set("inventory", Sprite{texture});

        inventory = entity_manager.add_entity(
            NameComponent{"inventory"},
            TransformComponent{
                {context.virtual_window_width / 2.f, context.virtual_window_height / 2.f},
                {0.f, 0.f},
                {1.f, 1.f},
                Coordinate::Type::Screen
            },
            InventoryComponent{slot_nums_[0] * slot_nums_[1]},
            SpriteComponent{"inventory"},
            RenderComponent{10},
            InventoryLayerTag{}
        );
    }

    // create inventory items entity
    {
        auto texture = sdl::SDL::create_texture(
            inventory_sizes_[0], inventory_sizes_[1],
            sdl::SDL::Color::Transparent,
            SDL_TEXTUREACCESS_TARGET
        );
        SpriteManager::instance().set("inventory_items", Sprite{texture});
        auto inventory_items = entity_manager.add_entity(
            inventory,
            NameComponent{"inventory_items"},
            TransformComponent{},
            InventoryComponent{slot_nums_[0] * slot_nums_[1]},
            SpriteComponent{"inventory_items"},
            RenderComponent{11},
            InventoryLayerTag{}
        );
    }

    // create inventory rarities entity
    {
        auto texture = sdl::SDL::create_texture(
            inventory_sizes_[0], inventory_sizes_[1],
            sdl::SDL::Color::Transparent,
            SDL_TEXTUREACCESS_TARGET
        );
        SpriteManager::instance().set("inventory_rarities", Sprite{texture});
        auto inventory_rarities = entity_manager.add_entity(
            inventory,
            NameComponent{"inventory_rarities"},
            TransformComponent{},
            InventoryComponent{slot_nums_[0] * slot_nums_[1]},
            SpriteComponent{"inventory_rarities"},
            RenderComponent{12},
            InventoryLayerTag{}
        );
    }

    // create inventory items count entity
    {
        auto texture = sdl::SDL::create_texture(
            inventory_sizes_[0], inventory_sizes_[1],
            sdl::SDL::Color::Transparent,
            SDL_TEXTUREACCESS_TARGET
        );
        SpriteManager::instance().set("inventory_items_count", Sprite{texture});
        auto inventory_rarities = entity_manager.add_entity(
            inventory,
            NameComponent{"inventory_items_count"},
            TransformComponent{},
            InventoryComponent{slot_nums_[0] * slot_nums_[1]},
            SpriteComponent{"inventory_items_count"},
            RenderComponent{13},
            InventoryLayerTag{}
        );
    }
}

void InventoryLayer::on_show() {
    for (auto entity : ecs.get_entities<InventoryLayerTag>()) {
        ecs.add_component(entity, RenderTag{});
    }
}

void InventoryLayer::on_hide() {
    for (auto entity : ecs.get_entities<InventoryLayerTag>()) {
        ecs.remove_components<RenderTag>(entity);
    }
}

void InventoryLayer::on_update() {
    auto& items = ecs.get_component<InventoryComponent>().items;

    // update slot item position
    for (auto [idx, entity] : std::views::enumerate(items)) {
        int i = idx % 10, j = idx / 10;
        if (ecs.has_entity(entity)) {
            auto& transform = ecs.get_component<TransformComponent>(entity);
            auto [x, y, w, h] = slot_rects_[i][j];
            transform.local.position = {x + w / 2.f, y + h / 2.f};
        }
    }

    // render item sprite to inventory_items texture
    {
        auto texture = SpriteManager::instance().get("inventory_items").texture;
        sdl::SDL::RenderTargetGuard guard(texture);
        sdl::SDL::RenderColorGuard color_guard(sdl::SDL::Color::Transparent);
        sdl::SDL::render_clear();

        for (auto [idx, entity] : std::views::enumerate(items)) {
            int i = idx % 10, j = idx / 10;
            if (ecs.has_entity(entity)) {
                const auto& item = ecs.get_component<ItemComponent>(entity);
                const auto& sprite = SpriteManager::instance().get(item.id);
                sdl::SDL::render_texture(sprite.texture, &sprite.rect, &slot_rects_[i][j]);
            }
        }
    }

    // render rarity sprite to inventory_rarities texture
    {
        auto texture = SpriteManager::instance().get("inventory_rarities").texture;
        sdl::SDL::RenderTargetGuard guard(texture);
        sdl::SDL::RenderColorGuard color_guard(sdl::SDL::Color::Transparent);
        sdl::SDL::render_clear();

        for (auto [idx, entity] : std::views::enumerate(items)) {
            int i = idx % 10, j = idx / 10;
            if (ecs.has_entity(entity)) {
                const auto& item = ecs.get_component<ItemComponent>(entity);
                const auto& rarity = ItemManager::instance().get(item.id).rarity;
                const auto& sprite = SpriteManager::instance().get(rfl::enum_to_string(rarity));
                sdl::SDL::render_texture(sprite.texture, &sprite.rect, &slot_rects_[i][j]);
            }
        }
    }

    // render items count sprite to inventory_items_count texture
    // {
    //     auto texture = SpriteManager::instance().get("inventory_items_count"_id).texture;
    //     sdl::SDL::RenderTargetGuard guard(texture);
    //     sdl::SDL::RenderColorGuard color_guard(sdl::SDL::Color::Transparent);
    //     sdl::SDL::render_clear();
    //     for (auto [idx, entity] : std::views::enumerate(items)) {
    //         int i = idx % 10, j = idx / 10;
    //         if (ecs.has_entity(entity)) {
    //             const auto& item = ecs.get_component<ItemComponent>(entity);
    //             if (item.count > 1) {
    //                 const auto& sprite = SpriteManager::instance().get(std::to_string(item.count));
    //                 SDL_FRect dst = {
    //                     slot_rects_[i][j].x + slot_rects_[i][j].w - sprite.rect.w,
    //                     slot_rects_[i][j].y + slot_rects_[i][j].h - sprite.rect.h,
    //                     sprite.rect.w,
    //                     sprite.rect.h
    //                 };
    //                 sdl::SDL::render_texture(sprite.texture, &sprite.rect, &dst);
    //             }
    //         }
    //     }
    // }
}

bool InventoryLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_I:
                case SDLK_ESCAPE: {
                    ecs.emplace_event<RemoveLayerEvent>();
                    return true;
                }
            }
        }
    }
    return false;
}

}  // namespace core
