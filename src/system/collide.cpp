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
#include <game/component/trap.hpp>
#include <game/component/animation.hpp>

namespace wheel {

void CollideSystem::execute_impl() {
    collide();
    bullet_out_of_boundary();
    bullet_collide_structure();
    trap_collide_enemy();
    pick_range();
    auto_pickup();
}

void CollideSystem::collide() {
    // friend collide enemy
    for (auto [entity0, _, collide0, position0, size0]
        : ecs.get_entity_and_components<FriendComponent, CollideComponent, PositionComponent, SizeComponent>()) {
        Rect<float> rect0 = {position0.vec, size0.vec / 2};
        for (auto [entity1, _, hp, position1, size1]
            : ecs.get_entity_and_components<EnemyComponent, HPComponent, PositionComponent, SizeComponent>()) {
            Rect<float> rect1 = {position1.vec, size1.vec / 2};
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
        Rect<float> rect0 = {position0.vec, size0.vec / 2};
        for (auto [entity1, _, hp, position1, size1]
            : ecs.get_entity_and_components<FriendComponent, HPComponent, PositionComponent, SizeComponent>()) {
            Rect<float> rect1 = {position1.vec, size1.vec / 2};
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
        Rect<float> rect = {position.vec, size.vec / 2};
        if (map.is_collision(rect)) {
            ecs.add_components<DelEntityTag>(bullet_entity, {});
        }
    }
}

void CollideSystem::trap_collide_enemy() {
    for (auto [trap_entity, trap, position0, size0] : ecs.get_entity_and_components<TrapComponent, PositionComponent, SizeComponent>()) {
        Rect<float> rect0 = {position0.vec, size0.vec / 2};
        std::vector<Entity> enemies;
        for (auto [enemy_entity, enemy, _, position1, size1] : ecs.get_entity_and_components<EnemyComponent, HPComponent, PositionComponent, SizeComponent>()) {
            Rect<float> rect1 = {position1.vec, size1.vec / 2};
            if (rect0.is_overlapping(rect1)) {
                enemies.emplace_back(enemy_entity);
                if (--trap.duration <= 0) {
                    ecs.add_components(trap_entity, DelEntityTag{});
                    break;
                }
            }
        }

        // add damage
        for (auto enemy_entity : enemies) {
            if (!ecs.has_components<DamageComponent>(enemy_entity)) {
                ecs.add_components(enemy_entity, DamageComponent{});
            }
            auto& damage = ecs.get_component<DamageComponent>(enemy_entity);
            damage.damages.emplace_back(trap.atk, trap_entity);
        }

        // cooldown
        if (enemies.size() && trap.cooldown > 0) {
            auto& animation = ecs.get_component<AnimationComponent>(trap_entity);
            animation.idx = 0;
            animation.counter = 0;
            animation.type = Animations::Type::ATTACK;
            int frames = animation.animations->frames();
            int sep = trap.cooldown / frames;
            timer_resource.add(sep + 1, frames, [trap_entity, &animation](int cnt) {
                if (!ecs.has_entity(trap_entity)) {
                    return;
                }
                if (cnt > 1) {
                    animation.idx = (animation.idx + 1) % animation.animations->frames();
                } else {
                    animation.idx = 0;
                    animation.counter = 0;
                    animation.type = Animations::Type::NORMAL;
                }
            }, true);

            auto trap = ecs.get_component<TrapComponent>(trap_entity);
            ecs.del_components<TrapComponent>(trap_entity);
            timer_resource.add(trap.cooldown, 1, [trap_entity, trap, &animation]() {
                if (ecs.has_entity(trap_entity)) {
                    ecs.add_components(trap_entity, trap);
                }
            });
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
                        VelocityComponent{(float)perk.pick_range + 100}
                    );
                }
            }
        }
    }
}

void CollideSystem::auto_pickup() {
    for (auto [entity0, item, position0, size0] : ecs.get_entity_and_components<ItemComponent, PositionComponent, SizeComponent>()) {
        Rect<float> rect0 = {position0.vec, size0.vec / 2};
        for (auto [_, inventory, position1, size1] : ecs.get_components<PlayerComponent, InventoryComponent, PositionComponent, SizeComponent>()) {
            Rect<float> rect1 = {position1.vec, size1.vec / 2};
            if (rect0.is_overlapping(rect1)) {
                std::visit([&](const auto& data) {
                    if (inventory.inventory.pick(data, item.count)) {
                        ecs.add_components(entity0, DelEntityTag{});
                    }
                }, item.data);
            }
        }
    }
}

}  // namespace wheel
