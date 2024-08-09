#include <game/system/collide.hpp>

#include <game/global.hpp>
#include <game/event.hpp>
#include <game/game_manager.hpp>
#include <game/map.hpp>

#include <game/component/position.hpp>
#include <game/component/direction.hpp>
#include <game/component/velocity.hpp>
#include <game/component/size.hpp>
#include <game/component/item.hpp>
#include <game/component/damage.hpp>
#include <game/component/invincible.hpp>
#include <game/component/enemy.hpp>
#include <game/component/friend.hpp>
#include <game/component/collide.hpp>
#include <game/component/tag.hpp>
#include <game/component/bullet.hpp>
#include <game/component/master.hpp>
#include <game/component/hp.hpp>
#include <game/component/inventory.hpp>
#include <game/component/perk.hpp>
#include <game/component/ai.hpp>
#include <game/component/player.hpp>

namespace wheel {

void CollideSystem::execute_impl() {
    collide();
    bullet_out_of_boundary();
    bullet_collide_structure();
    pick_range();
    auto_pickup();
}

void CollideSystem::collide() {
    // friend collide enemy
    for (auto [entity0, _, collide0, position0, size0]
        : ecs.get_entity_and_components<FriendComponent, CollideComponent, PositionComponent, SizeComponent>()) {
        Rect<double> rect0 = {position0.vec, {size0.w / 2., size0.h / 2.}};
        for (auto [entity1, _, hp, position1, size1]
            : ecs.get_entity_and_components<EnemyComponent, HPComponent, PositionComponent, SizeComponent>()) {
            Rect<double> rect1 = {position1.vec, {size1.w / 2., size1.h / 2.}};
            if (rect0.is_overlapping(rect1)) {
                // unique collide
                if (ecs.has_components<UniqueCollideComponent>(entity0)) {
                    auto& unique_collide = ecs.get_component<UniqueCollideComponent>(entity0);
                    if (unique_collide.last == entity1) {
                        continue;
                    }
                }

                // add damage
                if (!ecs.has_components<DamageComponent>(entity1)) {
                    ecs.add_components<DamageComponent>(entity1, {});
                }
                auto& damage = ecs.get_component<DamageComponent>(entity1);
                damage.damages.emplace_back(collide0.atk, entity0);

                // out of durability
                if (collide0.durability != -1 && !--collide0.durability) {
                    if (!ecs.has_components<DelEntityTag>(entity0)) {
                        ecs.add_components<DelEntityTag>(entity0, {});
                    }
                }

                // collide cooldown
                // TODO: delete CollideComponent may cause bugs
                if (collide0.cooldown > 0) {
                    auto collide = ecs.get_component<CollideComponent>(entity0);
                    timer_resource.add(30, 1, [=]() {
                        ecs.add_components(entity0, collide);
                    });
                    ecs.del_components<CollideComponent>(entity0);
                }

                // bullet unique collide
                if (ecs.has_components<BulletComponent>(entity0)) {
                    ecs.add_components<UniqueCollideComponent>(entity0, {entity1});
                }

                break;
            }
        }
    }

    // enemy collide friend
    for (auto [entity0, _, collide0, position0, size0]
        : ecs.get_entity_and_components<EnemyComponent, CollideComponent, PositionComponent, SizeComponent>()) {
        Rect<double> rect0 = {position0.vec, {size0.w / 2., size0.h / 2.}};
        for (auto [entity1, _, hp, position1, size1]
            : ecs.get_entity_and_components<FriendComponent, HPComponent, PositionComponent, SizeComponent>()) {
            Rect<double> rect1 = {position1.vec, {size1.w / 2., size1.h / 2.}};
            if (rect0.is_overlapping(rect1)) {
                // add damage
                if (!ecs.has_components<DamageComponent>(entity1)) {
                    ecs.add_components<DamageComponent>(entity1, {});
                }
                auto& damage = ecs.get_component<DamageComponent>(entity1);
                damage.damages.emplace_back(collide0.atk, entity0);

                // out of durability
                if (collide0.durability != -1 && !--collide0.durability) {
                    if (!ecs.has_components<DelEntityTag>(entity0)) {
                        ecs.add_components<DelEntityTag>(entity0, {});
                    }
                }

                break;
            }
        }
    }
}

void CollideSystem::bullet_out_of_boundary() {
    auto& map = Map::instance();
    for (auto [bullet_entity, bullet, position] : ecs.get_entity_and_components<BulletComponent, PositionComponent>()) {
        if (!map.is_in_bound(position.vec)) {
            ecs.add_components<DelEntityTag>(bullet_entity, {});
        }
    }
}

void CollideSystem::bullet_collide_structure() {
    auto& map = Map::instance();
    for (auto [bullet_entity, bullet, position, size] : ecs.get_entity_and_components<BulletComponent, PositionComponent, SizeComponent>()) {
        Rect<double> rect = {position.vec, {size.w / 2., size.h / 2.}};
        if (map.is_collision(rect)) {
            ecs.add_components<DelEntityTag>(bullet_entity, {});
        }
    }
}

// TODO: performance
void CollideSystem::pick_range() {
    for (auto [perk, inventory, position0] : ecs.get_components<PerkComponent, InventoryComponent, PositionComponent>()) {
        if (perk.pick_range > 0) {
            for (auto [entity, item, position1] : ecs.get_entity_and_components<ItemComponent, PositionComponent>()) {
                if (ecs.has_components<TrackNearestPlayerTag>(entity)) {
                    continue;
                }
                if (position0.vec.distance(position1.vec) <= perk.pick_range) {
                    ecs.add_components(
                        entity,
                        TrackNearestPlayerTag{},
                        DirectionComponent{},
                        VelocityComponent{perk.pick_range + 100.}
                    );
                }
            }
        }
    }
}

void CollideSystem::auto_pickup() {
    for (auto [entity0, item, position0, size0] : ecs.get_entity_and_components<ItemComponent, PositionComponent, SizeComponent>()) {
        Rect<double> rect0 = {position0.vec, {size0.w / 2., size0.h / 2.}};
        for (auto [_, inventory, position1, size1] : ecs.get_components<PlayerComponent, InventoryComponent, PositionComponent, SizeComponent>()) {
            Rect<double> rect1 = {position1.vec, {size1.w / 2., size1.h / 2.}};
            if (rect0.is_overlapping(rect1)) {
                std::visit([&](const auto& data) {
                    if (inventory.inventory.pick(data, item.count)) {
                        ecs.add_components<DelEntityTag>(entity0, {});
                    }
                }, item.data);
            }
        }
    }
}

}  // namespace wheel
