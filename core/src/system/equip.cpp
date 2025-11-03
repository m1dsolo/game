#include <core/system/equip.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/item.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/collider.hpp>
#include <core/component/master.hpp>
#include <core/component/sprite.hpp>
#include <core/component/inventory.hpp>
#include <core/component/render.hpp>
#include <core/tag/render.hpp>
#include <core/tag/save.hpp>
#include <core/tag/game_layer.hpp>
#include <core/event/equip.hpp>
#include <core/entity_event/remove_entity.hpp>
#include <core/resource/hotbar.hpp>

#include <ecs/ecs.hpp>

namespace core {

void EquipSystem::operator()(wheel::ECS& ecs) {
    auto& hotbar = ecs.get_resource<HotbarResource>();
    for (const auto [item_id, slot_idx] : ecs.get_events<EquipEvent>()) {
        const auto& item_info = ItemManager::instance().get(item_id);
        if (item_info.components.range_attack.has_value()) {
            const auto& range_attack = item_info.components.range_attack.value();
            const auto& reload = item_info.components.reload.value();
            float range = range_attack.range;
            auto user = ecs.get_entity<InventoryComponent>();
            auto entity = EntityManager::instance().add_entity(
                user,
                NameComponent{"range_attack"},
                TransformComponent{.local = {.size = {range, range}}},
                ColliderComponent{
                    wheel::Circle<float>{range / 2.f},
                    ColliderLayer::Trigger,
                    ColliderLayer::Enemy
                },
                range_attack,
                reload,
                MasterComponent{user},
                SpriteComponent{"range_attack"},
                RenderComponent{3},
                RenderTag{},
                SaveTag{},
                GameLayerTag{}
            );
            auto& re = ecs.get_component<ReloadComponent>(entity);
            re.current_ammo = re.max_ammo;

            hotbar.entities[slot_idx].emplace_back(entity);
        }
    }

    for (const auto [item_id, slot_idx] : ecs.get_events<UnequipEvent>()) {
        for (auto entity : hotbar.entities[slot_idx]) {
            ecs.add_entity_event(entity, RemoveEntityEvent{});
        }
        hotbar.entities[slot_idx].clear();
    }
}

}  // namespace core
