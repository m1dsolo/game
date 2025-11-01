#include <core/layer/item_info.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/item.hpp>
#include <core/manager/sprite.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/sprite.hpp>
#include <core/component/item.hpp>
#include <core/component/render.hpp>
#include <core/tag/render.hpp>
#include <core/event/layer.hpp>
#include <core/entity_event/remove_entity.hpp>
#include <core/sdl_user_event/change_selected_slot.hpp>

#include <sdl/sdl.hpp>

#include <iostream>

namespace core {

void ItemInfoLayer::on_register() {
    item_info_entity_ = EntityManager::instance().add_entity(
        selected_slot_border,
        NameComponent{"item_info"},
        TransformComponent{.local = {.size = {300.f, 512.f}}, .anchor = {2.f, 2.f}},
        SpriteComponent{""},
        RenderComponent{15}
    );
}

void ItemInfoLayer::on_attach() {
    ecs.add_component(item_info_entity_, RenderTag{});
    update_item_info_();
}

void ItemInfoLayer::on_detach() {
    ecs.remove_component<RenderTag>(item_info_entity_);
}

bool ItemInfoLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_RETURN:
                case SDLK_ESCAPE: {
                    ecs.emplace_event<RemoveLayerEvent>();
                    return true;
                }
            }
            break;
        }
        case SDL_EVENT_CHANGE_SELECTED_SLOT: {
            update_item_info_();
            return true;
        }
    }
    return false;
}

void ItemInfoLayer::update_item_info_() {
    auto& sprite = ecs.get_component<SpriteComponent>(item_info_entity_);
    sprite.sprite = &SpriteManager::instance().get(wheel::ID("item_info") ^ item_id);
}

}  // namespace core
