#include <game/system/collide.hpp>

#include <game/global.hpp>
#include <game/event.hpp>

#include <game/component/position.hpp>
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

namespace wheel {

void CollideSystem::execute() {
    if (game_resource.paused) {
        return;
    }

    collide();
    bullet_out_of_boundary();
    auto_pickup();
}

void CollideSystem::collide() {
    // friend collide enemy
    for (auto [entity0, _, collide0, position0, size0]
        : ecs.get_entity_and_components<FriendComponent, CollideComponent, PositionComponent, SizeComponent>()) {
        Rect<double> rect0 = {{position0.vec.x, position0.vec.y}, {(double)size0.w / 2, (double)size0.h / 2}};
        for (auto [entity1, _, hp, position1, size1]
            : ecs.get_entity_and_components<EnemyComponent, HPComponent, PositionComponent, SizeComponent>()) {
            Rect<double> rect1 = {{position1.vec.x, position1.vec.y}, {(double)size1.w / 2, (double)size1.h / 2}};
            if (rect0.intersection(rect1) > 0) {
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
        Rect<double> rect0 = {{position0.vec.x, position0.vec.y}, {(double)size0.w / 2, (double)size0.h / 2}};
        for (auto [entity1, _, hp, position1, size1]
            : ecs.get_entity_and_components<FriendComponent, HPComponent, PositionComponent, SizeComponent>()) {
            Rect<double> rect1 = {{position1.vec.x, position1.vec.y}, {(double)size1.w / 2, (double)size1.h / 2}};
            if (rect0.intersection(rect1) > 0) {
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
    for (auto [bullet_entity, bullet, position, size] : ecs.get_entity_and_components<BulletComponent, PositionComponent, SizeComponent>()) {
        int u = size.h;
        int d = config_resource.h - size.h;
        int l = size.w;
        int r = config_resource.w - size.w;
        if (position.vec.y <= u || position.vec.y >= d || position.vec.x <= l || position.vec.x >= r) {
            ecs.add_components<DelEntityTag>(bullet_entity, {});
        }
    }
}

void CollideSystem::auto_pickup() {
    // auto pick item
    for (auto [entity0, item, position0, size0] : ecs.get_entity_and_components<ItemComponent, PositionComponent, SizeComponent>()) {
        Rect<double> rect0 = {{position0.vec.x, position0.vec.y}, {(double)size0.w / 2, (double)size0.h / 2}};
        for (auto [inventory, position1, size1] : ecs.get_components<InventoryComponent, PositionComponent, SizeComponent>()) {
            Rect<double> rect1 = {{position1.vec.x, position1.vec.y}, {(double)size1.w / 2, (double)size1.h / 2}};
            if (rect0.intersection(rect1) > 0) {
                if (inventory.inventory.pick(item.name, item.count)) {
                    ecs.add_components<DelEntityTag>(entity0, {});
                }
            }
        }
    }
}

}  // namespace wheel
