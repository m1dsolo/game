#include <core/system/drop_item.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/collider.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>
#include <core/component/sprite.hpp>
#include <core/component/render.hpp>
#include <core/component/loot.hpp>
#include <core/component/collider.hpp>
#include <core/component/item.hpp>
#include <core/tag/render.hpp>
#include <core/tag/game_layer.hpp>
#include <core/tag/render.hpp>
#include <core/entity_event/death.hpp>

#include <wheel/id.hpp>
#include <wheel/random.hpp>

namespace core {

void DropItemSystem::operator()(wheel::ECS& ecs) {
    for (auto [entity, event, loot, transform] : ecs.get_entity_and_components<DeathEvent, LootComponent, TransformComponent>()) {
        for (const auto& group : loot.groups) {
            if (wheel::Random::instance().uniform<float>(0.f, 1.f) <= group.chance) {
                int total_weight = 0;
                for (const auto& item : group.items) {
                    total_weight += item.weight;
                }
                int pick = wheel::Random::instance().uniform(1, total_weight);
                for (const auto& item : group.items) {
                    pick -= item.weight;
                    if (pick <= 0) {
                        int count = wheel::Random::instance().uniform(item.count.first, item.count.second);
                        wheel::ID id = item.name;
                        EntityManager::instance().add_entity(
                            NameComponent{item.name},
                            TransformComponent{.local={transform.global.position, {16.f, 16.f}}},
                            ColliderComponent{wheel::Rect<float>{{0.f, 0.f}, {16.f, 16.f}}},
                            SpriteComponent{id},
                            RenderComponent{4},
                            ItemComponent{id + static_cast<int>(item.rarity), count},
                            RenderTag{},
                            GameLayerTag{}
                        );
                        break;
                    }
                }
            }
        }
    }
}

}  // namespace core
