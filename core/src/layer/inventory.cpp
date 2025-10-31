#include <core/layer/inventory.hpp>
#include <core/global.hpp>
#include <core/layer/item_info.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/sprite.hpp>
#include <core/manager/item.hpp>
#include <core/manager/audio.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/sprite.hpp>
#include <core/component/render.hpp>
#include <core/component/item.hpp>
#include <core/component/inventory.hpp>
#include <core/tag/input.hpp>
#include <core/tag/render.hpp>
#include <core/tag/inventory_layer.hpp>
#include <core/tag/save.hpp>
#include <core/resource/context.hpp>
#include <core/resource/inventory.hpp>
#include <core/event/layer.hpp>
#include <core/entity_event/remove_entity.hpp>

#include <rfl/enums.hpp>

namespace core {

void InventoryLayer::on_register() {
    auto& entity_manager = EntityManager::instance();
    const auto& context = ecs.get_resource<ContextResource>();
    const auto& slot_nums = ecs.get_resource<InventoryResource>().slot_nums;

    auto inventory_background = entity_manager.add_entity(
        NameComponent{"inventory_background_layer"},
        TransformComponent{
            {{context.virtual_window_width / 2.f, context.virtual_window_height / 2.f}},
            {},
            Coordinate::Type::Screen
        },
        SpriteComponent{"inventory_background_layer"},
        RenderComponent{10},
        InventoryLayerTag{}
    );

    entity_manager.add_entity(
        inventory_background,
        NameComponent{"inventory_items_layer"},
        TransformComponent{},
        SpriteComponent{"inventory_items_layer"},
        RenderComponent{11},
        InventoryLayerTag{}
    );

    // create inventory rarities entity
    {
        auto inventory_rarities = entity_manager.add_entity(
            inventory_background,
            NameComponent{"inventory_rarities_layer"},
            TransformComponent{},
            SpriteComponent{"inventory_rarities_layer"},
            RenderComponent{12},
            InventoryLayerTag{}
        );
    }

    // create inventory items count entity
    {
        auto inventory_rarities = entity_manager.add_entity(
            inventory_background,
            NameComponent{"inventory_items_count_layer"},
            TransformComponent{},
            SpriteComponent{"inventory_items_count_layer"},
            RenderComponent{13},
            InventoryLayerTag{}
        );
    }

    // create selected slot border entity
    {
        selected_slot_border_ = entity_manager.add_entity(
            inventory_background,
            NameComponent{"selected_slot_border"},
            TransformComponent{.anchor = {-0.5f, -0.5f}},
            SpriteComponent{"selected_slot_border"},
            RenderComponent{14},
            InventoryLayerTag{}
        );
        ItemInfoLayer::selected_slot_border = selected_slot_border_;
    }
}

void InventoryLayer::on_attach() {
    selected_idx_[0] = selected_idx_[1] = 0;
    for (auto entity : ecs.get_entities<InventoryLayerTag>()) {
        ecs.add_component(entity, RenderTag{});
    }
}

void InventoryLayer::on_detach() {
    for (auto entity : ecs.get_entities<InventoryLayerTag>()) {
        ecs.remove_component<RenderTag>(entity);
    }
}

void InventoryLayer::on_update() {
    const auto& slot_rects = ecs.get_resource<InventoryResource>().slot_rects;
    auto& items = ecs.get_component<InventoryComponent>().items;

    // render item sprite to inventory_items_layer texture
    {
        auto texture = SpriteManager::instance().get("inventory_items_layer").texture;
        sdl::SDL::RenderTargetGuard guard(texture);
        sdl::SDL::RenderColorGuard color_guard(sdl::SDL::Color::Transparent);
        sdl::SDL::render_clear();

        for (auto [idx, entity] : std::views::enumerate(items)) {
            int i = idx % 10, j = idx / 10;
            if (ecs.has_entity(entity)) {
                const auto& item = ecs.get_component<ItemComponent>(entity);
                const auto& sprite = SpriteManager::instance().get(item.id);
                sdl::SDL::render_texture(sprite.texture, &sprite.rect, &slot_rects[i][j]);
            }
        }
    }

    // render rarity sprite to inventory_rarities_layer texture
    {
        auto texture = SpriteManager::instance().get("inventory_rarities_layer").texture;
        sdl::SDL::RenderTargetGuard guard(texture);
        sdl::SDL::RenderColorGuard color_guard(sdl::SDL::Color::Transparent);
        sdl::SDL::render_clear();

        for (auto [idx, entity] : std::views::enumerate(items)) {
            int i = idx % 10, j = idx / 10;
            if (ecs.has_entity(entity)) {
                const auto& item = ecs.get_component<ItemComponent>(entity);
                const auto& rarity = ItemManager::instance().get(item.id).rarity;
                const auto& sprite = SpriteManager::instance().get(rfl::enum_to_string(rarity));
                sdl::SDL::render_texture(sprite.texture, &sprite.rect, &slot_rects[i][j]);
            }
        }
    }

    // render items count sprite to inventory_items_count_layer texture
    {
        auto texture = SpriteManager::instance().get("inventory_items_count_layer").texture;
        sdl::SDL::RenderTargetGuard guard(texture);
        sdl::SDL::RenderColorGuard color_guard(sdl::SDL::Color::Transparent);
        sdl::SDL::render_clear();
        for (auto [idx, entity] : std::views::enumerate(items)) {
            int i = idx % 10, j = idx / 10;
            if (ecs.has_entity(entity)) {
                const auto& item = ecs.get_component<ItemComponent>(entity);
                if (item.count > 1) {
                    const auto& sprite = SpriteManager::instance().get(std::to_string(item.count));
                    SDL_FRect dst = {
                        slot_rects[i][j].x + slot_rects[i][j].w - sprite.rect.w,
                        slot_rects[i][j].y + slot_rects[i][j].h - sprite.rect.h,
                        sprite.rect.w,
                        sprite.rect.h
                    };
                    sdl::SDL::render_texture(sprite.texture, &sprite.rect, &dst);
                }
            }
        }
    }

    // update selected slot border position
    {
        auto& transform = ecs.get_component<TransformComponent>(selected_slot_border_);
        auto [i, j] = selected_idx_;
        auto [x, y, w, h] = slot_rects[i][j];
        transform.local.position = {x + w / 2.f, y + h / 2.f};
    }
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
                case SDLK_W: {
                    select_slot_(0, -1);
                    return true;
                }
                case SDLK_S: {
                    select_slot_(0, 1);
                    return true;
                }
                case SDLK_A: {
                    select_slot_(-1, 0);
                    return true;
                }
                case SDLK_D: {
                    select_slot_(1, 0);
                    return true;
                }
                case SDLK_RETURN: {
                    auto [m, n]  = ecs.get_resource<InventoryResource>().slot_nums;
                    const auto& items = ecs.get_component<InventoryComponent>().items;
                    auto item_entity = items[selected_idx_[1] * m + selected_idx_[0]];
                    if (item_entity != wheel::NullEntity) {
                        ItemInfoLayer::item_entity = item_entity;
                        AudioManager::instance().play("hover_button");
                        ecs.emplace_event<AddLayerEvent>("ItemInfoLayer");
                    }
                    return true;
                }
            }
        }
    }
    return false;
}

void InventoryLayer::select_slot_(int delta_x, int delta_y) {
    auto [m, n]  = ecs.get_resource<InventoryResource>().slot_nums;
    selected_idx_[0] = (selected_idx_[0] + delta_x + m) % m;
    selected_idx_[1] = (selected_idx_[1] + delta_y + n) % n;
    const auto& items = ecs.get_component<InventoryComponent>().items;
    ItemInfoLayer::item_entity = items[selected_idx_[1] * m + selected_idx_[0]];
    AudioManager::instance().play("hover_button");
}

}  // namespace core
