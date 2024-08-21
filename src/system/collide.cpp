#include <game/system/collide.hpp>

#include <game/global.hpp>
#include <game/event.hpp>
#include <game/game_manager.hpp>
#include <game/collision_manager.hpp>
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

CollideSystem::CollideSystem()
    : BaseSystem(false) {}

void CollideSystem::execute_impl() {
    update_collision_manager();
    pick_range();
    collide();
    bullet_out_of_boundary();
    bullet_collide_structure();
}

void CollideSystem::update_collision_manager() {
    CollisionManager::instance().update();
}

void CollideSystem::collide() {
    auto& collision_manager = CollisionManager::instance();
    for (auto [entity0, entity1] : collision_manager.find_all_dynamic_intersections()) {
        // collide damage
        bool f0 = ecs.has_components<FriendComponent>(entity0);
        bool e0 = ecs.has_components<EnemyComponent>(entity0);
        bool f1 = ecs.has_components<FriendComponent>(entity1);
        bool e1 = ecs.has_components<EnemyComponent>(entity1);
        bool hostile = f0 && e1 || f1 && e0;
        if (hostile) {
            if (ecs.has_components<CollideComponent>(entity0)) {
                collide_damage(entity0, entity1);
            }
            if (ecs.has_components<CollideComponent>(entity1)) {
                collide_damage(entity1, entity0);
            }
        }

        // pick
        bool p0 = ecs.has_components<PlayerComponent>(entity0);
        bool i0 = ecs.has_components<ItemComponent>(entity0);
        bool p1 = ecs.has_components<PlayerComponent>(entity1);
        bool i1 = ecs.has_components<ItemComponent>(entity1);
        if (p0 && i1) {
            pick(entity0, entity1);
        } else if (p1 && i0) {
            pick(entity1, entity0);
        }
    }
}

void CollideSystem::bullet_out_of_boundary() {
    auto& map = Map::instance();
    for (auto [bullet_entity, bullet, position] : ecs.get_entity_and_components<BulletComponent, PositionComponent>()) {
        if (!map.is_in_bound(position.vec)) {
            ecs.add_components(bullet_entity, DelEntityTag{});
        }
    }
}

void CollideSystem::bullet_collide_structure() {
    auto& collision_manager = CollisionManager::instance();
    for (auto [entity, _] : ecs.get_entity_and_components<BulletComponent>()) {
        if (collision_manager.is_collision_with_static(entity)) {
            ecs.add_components(entity, DelEntityTag{});
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
                        VelocityComponent{(float)perk.pick_range + 200}
                    );
                }
            }
        }
    }
}

void CollideSystem::collide_damage(Entity entity0, Entity entity1) {
    if (ecs.has_components<UniqueCollideComponent>(entity0)) {
        auto& unique_collide = ecs.get_component<UniqueCollideComponent>(entity0);
        if (unique_collide.last == entity1) {
            return;
        }
    }

    auto& collide = ecs.get_component<CollideComponent>(entity0);
    // add damage
    if (!ecs.has_components<DamageComponent>(entity1)) {
        ecs.add_components(entity1, DamageComponent{});
    }
    auto& damage = ecs.get_component<DamageComponent>(entity1);
    damage.damages.emplace_back(collide.atk, entity0);

    // out of durability
    if (collide.durability != -1 && !--collide.durability) {
        if (!ecs.has_components<DelEntityTag>(entity0)) {
            ecs.add_components(entity0, DelEntityTag{});
            return;
        }
    }

    // collide cooldown
    if (collide.cooldown > 0) {
        auto collide = ecs.get_component<CollideComponent>(entity0);
        timer_resource.add(collide.cooldown, 1, [&, entity0, collide]() {
            if (ecs.has_entity(entity0)) {
                ecs.add_components(entity0, std::move(collide));
            }
        });
        ecs.del_components<CollideComponent>(entity0);
    }

    // bullet unique collide
    if (ecs.has_components<BulletComponent>(entity0)) {
        ecs.add_components(entity0, UniqueCollideComponent{entity1});
    }

    // trap
    // TODO: rewrite
    if (ecs.has_components<TrapComponent>(entity0)) {
        auto& animation = ecs.get_component<AnimationComponent>(entity0);
        animation.idx = 0;
        animation.counter = 0;
        animation.type = Animations::Type::ATTACK;
        int frames = animation.animations->frames();
        int sep = collide.cooldown / frames;
        timer_resource.add(sep + 1, frames, [entity0, &animation](int cnt) {
            if (!ecs.has_entity(entity0)) {
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
    }
}

void CollideSystem::pick(Entity entity0, Entity entity1) {
    auto& inventory = ecs.get_component<InventoryComponent>(entity0).inventory;
    auto& item = ecs.get_component<ItemComponent>(entity1);
    std::visit([&](const auto& data) {
        if (inventory.pick(data, item.count)) {
            ecs.add_components(entity1, DelEntityTag{});
        }
    }, item.data);
}

}  // namespace wheel
