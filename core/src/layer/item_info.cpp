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

#include <sdl/sdl.hpp>

namespace core {

void ItemInfoLayer::on_attach() {
    item_info_entity_ = EntityManager::instance().add_entity(
        selected_slot_border,
        NameComponent{"item_info"},
        TransformComponent{.local = {.size = {200.f, 300.f}}, .anchor = {2.f, 2.f}},
        SpriteComponent{""},
        RenderComponent{15},
        RenderTag{}
    );
}

void ItemInfoLayer::on_detach() {
    ecs.add_entity_event(item_info_entity_, RemoveEntityEvent{});
}

void ItemInfoLayer::on_update() {
    auto& sprite = ecs.get_component<SpriteComponent>(item_info_entity_);
    if (item_entity == wheel::NullEntity) {
        std::cout << "[ItemInfoLayer] item_entity is NullEntity!" << std::endl;
        sprite.sprite = &SpriteManager::instance().get("");
    } else {
        auto item_id = ecs.get_component<ItemComponent>(item_entity).id;
        auto& item_name = ItemManager::instance().get(item_id).name;
        sprite.sprite = &SpriteManager::instance().get("item_info_" + item_name);
    }
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
        }
    }
    return false;
}

}  // namespace core
