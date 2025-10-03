#include <core/system/drop_item.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/component/name.hpp>
#include <core/component/loot.hpp>
#include <core/component/render.hpp>
#include <core/component/item.hpp>
#include <core/event/death.hpp>

#include <wheel/random.hpp>

namespace core {

void DropItemSystem::operator()() {
    for (auto [source, target] : ecs.get_events<DeathEvent>()) {
        if (ecs.has_component<LootComponent>(target)) {
            for (const auto& group : ecs.get_component<LootComponent>(target).groups) {
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
                            EntityManager::instance().add_entity(
                                NameComponent{item.name},
                                TransformComponent{
                                    ecs.get_component<TransformComponent>(target).global.position,
                                    {16.f, 16.f}
                                },
                                SpriteComponent{item.name},
                                ColliderComponent{wheel::Rect<float>{{0.f, 0.f}, {16.f, 16.f}}},
                                RenderComponent{1},
                                ItemComponent{item.name, count}
                            );
                            break;
                        }
                    }
                }
            }
        }
    }
}

}  // namespace core
