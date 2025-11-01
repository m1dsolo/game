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
#include <core/event/layer.hpp>
#include <core/entity_event/remove_entity.hpp>
#include <core/sdl_user_event/update_slot.hpp>
#include <core/sdl_user_event/change_selected_slot.hpp>

#include <rfl/enums.hpp>

namespace core {

void InventoryLayer::on_register() {
    auto& entity_manager = EntityManager::instance();
    const auto& context = ecs.get_resource<ContextResource>();
    auto [n, m] = slot_nums_;

    const float inventory_sizes[2] = {
        n * (slot_sizes_[0] + slot_spacing_sizes_[0]) + slot_spacing_sizes_[0],
        m * (slot_sizes_[1] + slot_spacing_sizes_[1]) + slot_spacing_sizes_[1]
    };
    slot_rects_.resize(n, std::vector<SDL_FRect>(m));

    // create inventory items layer entity
    wheel::Entity inventory_background;
    {
        auto texture = sdl::SDL::create_texture(
            inventory_sizes[1], inventory_sizes[0],
            sdl::SDL::Color::White,
            SDL_TEXTUREACCESS_TARGET
        );
        sdl::SDL::RenderTargetGuard guard(texture);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                slot_rects_[i][j] = {
                    j * (slot_sizes_[1] + slot_spacing_sizes_[1]) + slot_spacing_sizes_[1],
                    i * (slot_sizes_[0] + slot_spacing_sizes_[0]) + slot_spacing_sizes_[0],
                    slot_sizes_[1],
                    slot_sizes_[0],
                };
                sdl::SDL::render_rect(&slot_rects_[i][j], sdl::SDL::Color::Black);
            }
        }
        SpriteManager::instance().set("inventory_background_layer", Sprite{texture});

        inventory_background = entity_manager.add_entity(
            NameComponent{"inventory_background_layer"},
            TransformComponent{
                .local={{context.virtual_window_width / 2.f, context.virtual_window_height / 2.f}},
                .type=Coordinate::Type::Screen
            },
            SpriteComponent{"inventory_background_layer"},
            RenderComponent{10},
            InventoryLayerTag{}
        );
    }

    slot_item_entities_.resize(n, std::vector<wheel::Entity>(m));
    slot_item_rarity_entities_.resize(n, std::vector<wheel::Entity>(m));
    slot_item_num_entities_.resize(n, std::vector<wheel::Entity>(m));

    // create inventory item slot entities
    {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                auto [x, y, w, h] = slot_rects_[i][j];
                slot_item_entities_[i][j] = entity_manager.add_entity(
                    inventory_background,
                    NameComponent{std::format("slot_item_{}_{}", i, j)},
                    TransformComponent{
                        .local = {.position = {x + w / 2.f, y + h / 2.f}, .size = {w, h}},
                        .anchor = {-0.5f, -0.5f}
                    },
                    SpriteComponent{""},
                    RenderComponent{11},
                    InventoryLayerTag{}
                );

                slot_item_rarity_entities_[i][j] = entity_manager.add_entity(
                    inventory_background,
                    NameComponent{std::format("slot_item_rarity_{}_{}", i, j)},
                    TransformComponent{
                        .local = {.position = {x + w / 2.f, y + h / 2.f}, .size = {w, h}},
                        .anchor = {-0.5f, -0.5f}
                    },
                    SpriteComponent{""},
                    RenderComponent{12},
                    InventoryLayerTag{}
                );

                slot_item_num_entities_[i][j] = entity_manager.add_entity(
                    inventory_background,
                    NameComponent{std::format("slot_item_num_{}_{}", i, j)},
                    TransformComponent{
                        .anchor = {-0.5f, -0.5f}
                    },
                    SpriteComponent{""},
                    RenderComponent{13},
                    InventoryLayerTag{}
                );
            }
        }
    }

    // create inventory selected slot border entity
    {
        auto texture = sdl::SDL::create_texture(
            slot_sizes_[0], slot_sizes_[1],
            sdl::SDL::Color::Transparent,
            SDL_TEXTUREACCESS_TARGET
        );
        sdl::SDL::RenderTargetGuard guard(texture);
        SDL_FRect dst = {0.f, 0.f, slot_sizes_[0], slot_sizes_[1]};
        sdl::SDL::render_rect(&dst, sdl::SDL::Color::Green, 5.f);
        SpriteManager::instance().set("selected_slot_border", Sprite{texture});

        selected_slot_border_ = entity_manager.add_entity(
            inventory_background,
            NameComponent{"selected_slot_border"},
            TransformComponent{
                .local = {.position = {slot_rects_[0][0].x + slot_rects_[0][0].w / 2.f, slot_rects_[0][0].y + slot_rects_[0][0].h / 2.f}},
                .anchor = {-0.5f, -0.5f}
            },
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

    for (int i = 0; i < slot_nums_[0]; ++i) {
        for (int j = 0; j < slot_nums_[1]; ++j) {
            update_sprite_(i, j);
        }
    }
}

void InventoryLayer::on_detach() {
    for (auto entity : ecs.get_entities<InventoryLayerTag>()) {
        ecs.remove_component<RenderTag>(entity);
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
                    select_slot_(-1, 0);
                    return true;
                }
                case SDLK_S: {
                    select_slot_(1, 0);
                    return true;
                }
                case SDLK_A: {
                    select_slot_(0, -1);
                    return true;
                }
                case SDLK_D: {
                    select_slot_(0, 1);
                    return true;
                }
                case SDLK_RETURN: {
                    const auto& slots = ecs.get_component<InventoryComponent>().slots;
                    auto item_id = slots[selected_idx_[0] * slot_nums_[1] + selected_idx_[1]].item_id;
                    if (item_id != "") {
                        ItemInfoLayer::item_id = item_id;
                        AudioManager::instance().play("hover_button");
                        ecs.emplace_event<AddLayerEvent>("ItemInfoLayer");
                    }
                    return true;
                }
            }
            break;
        }
        case SDL_EVENT_UPDATE_SLOT: {
            auto idx = event.user.code;
            update_sprite_(idx / slot_nums_[1], idx % slot_nums_[1]);
            return true;
        }
    }
    return false;
}

void InventoryLayer::select_slot_(int di, int dj) {
    auto [n, m] = slot_nums_;

    // update selected slot index
    auto& [i, j] = selected_idx_;
    i = (i + di + n) % n;
    j = (j + dj + m) % m;

    // update selected slot border position
    auto& transform = ecs.get_component<TransformComponent>(selected_slot_border_);
    auto [x, y, w, h] = slot_rects_[i][j];
    transform.local.position = {x + w / 2.f, y + h / 2.f};

    // pass selected item to ItemInfoLayer
    const auto& slots = ecs.get_component<InventoryComponent>().slots;
    ItemInfoLayer::item_id = slots[selected_idx_[0] * m + selected_idx_[1]].item_id;

    // push change selected slot event
    SDL_Event event;
    event.type = SDL_EVENT_CHANGE_SELECTED_SLOT;
    SDL_PushEvent(&event);

    // play sound effect
    AudioManager::instance().play("hover_button");
}

void InventoryLayer::update_sprite_(int i, int j) {
    auto& slot = ecs.get_component<InventoryComponent>().slots[i * slot_nums_[1] + j];

    {
        auto& sprite = ecs.get_component<SpriteComponent>(slot_item_entities_[i][j]);
        sprite.sprite = &SpriteManager::instance().get(slot.item_id);
    }

    {
        auto& sprite = ecs.get_component<SpriteComponent>(slot_item_rarity_entities_[i][j]);
        if (slot.item_id != "") {
            auto rarity = ItemManager::instance().get(slot.item_id).rarity;
            sprite.sprite = &SpriteManager::instance().get(rfl::enum_to_string(rarity));
        } else {
            sprite.sprite = &SpriteManager::instance().get("");
        }
    }

    {
        auto& transform = ecs.get_component<TransformComponent>(slot_item_num_entities_[i][j]);
        auto& sprite = ecs.get_component<SpriteComponent>(slot_item_num_entities_[i][j]);
        if (slot.item_id != "" && slot.num > 1) {
            sprite.sprite = &SpriteManager::instance().get(std::to_string(slot.num));
            auto [x, y, w, h] = slot_rects_[i][j];
            float text_w = sprite.sprite->rect.w;
            float text_h = sprite.sprite->rect.h;
            transform.local.position = {
                x + w - text_w / 2.f - 8.f,
                y + h - text_h / 2.f
            };
            transform.local.size = {text_w, text_h};
        } else {
            sprite.sprite = &SpriteManager::instance().get("");
        }
    }
}

}  // namespace core
