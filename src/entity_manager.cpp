#include <game/entity_manager.hpp>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

#include <wheel/json.hpp>

#include <game/global.hpp>
#include <game/utils.hpp>
#include <game/animation_manager.hpp>
#include <game/texture_manager.hpp>
#include <game/item_manager.hpp>
#include <game/behavior_tree_manager.hpp>
#include <game/collision_manager.hpp>
#include <game/scene_manager.hpp>
#include <game/map.hpp>

#include <game/item/tower.hpp>
#include <game/item/structure.hpp>
#include <game/item/trap.hpp>
#include <game/item/gun.hpp>

#include <game/component/size.hpp>
#include <game/component/position.hpp>
#include <game/component/velocity.hpp>
#include <game/component/direction.hpp>
#include <game/component/animation.hpp>
#include <game/component/texture.hpp>
#include <game/component/hp.hpp>
#include <game/component/collide.hpp>
#include <game/component/reward.hpp>
#include <game/component/level.hpp>
#include <game/component/ai.hpp>
#include <game/component/player.hpp>
#include <game/component/enemy.hpp>
#include <game/component/friend.hpp>
#include <game/component/actions.hpp>
#include <game/component/move.hpp>
#include <game/component/input.hpp>
#include <game/component/item.hpp>
#include <game/component/inventory.hpp>
#include <game/component/bullet.hpp>
#include <game/component/master.hpp>
#include <game/component/self.hpp>
#include <game/component/tower.hpp>
#include <game/component/health_bar.hpp>
#include <game/component/perk.hpp>
#include <game/component/aim_direction.hpp>
#include <game/component/collider.hpp>
#include <game/component/trap.hpp>

#include <game/resource/enemy.hpp>

namespace wheel {

EntityManager::EntityManager() {
    parse_enemy_json();

    cache_bullet("bullet_shell");
    cache_tower("archer_tower");
    cache_structure("floor");
    cache_structure("wall");
    cache_structure("door");
    cache_trap("peaks");
    cache_health_bar();
}

Entity EntityManager::create_player(const std::string& name, bool self) {
    auto [x0, y0, x1, y1] = Map::instance().game_rect();

    EntityTemplate t;
    t[typeid(PlayerComponent)] = PlayerComponent{};
    t[typeid(PerkComponent)] = PerkComponent{};
    t[typeid(AnimationComponent)] = AnimationComponent{name};
    t[typeid(TextureComponent)] = TextureComponent{};
    t[typeid(PositionComponent)] = PositionComponent{(float)(x0 + x1) / 2, (float)(y0 + y1) / 2};
    t[typeid(SizeComponent)] = SizeComponent{{48, 48}};
    t[typeid(VelocityComponent)] = VelocityComponent{100, 100};
    t[typeid(DirectionComponent)] = DirectionComponent{};
    t[typeid(AimDirectionComponent)] = AimDirectionComponent{};
    t[typeid(HPComponent)] = HPComponent{3};
    t[typeid(LevelComponent)] = LevelComponent{1, 0};
    t[typeid(ActionsComponent)] = ActionsComponent{};
    t[typeid(MoveComponent)] = MoveComponent{};
    t[typeid(FriendComponent)] = FriendComponent{};
    t[typeid(InputComponent)] = InputComponent{};
    t[typeid(ColliderComponent)] = ColliderComponent{};
    if (self) {
        t[typeid(SelfComponent)] = SelfComponent{};
    }

    Entity entity = ecs.add_entity(t);

    GameUtils::emplace_action<QuitAction>(entity);
    GameUtils::emplace_action<MoveUpAction>(entity);
    GameUtils::emplace_action<MoveDownAction>(entity);
    GameUtils::emplace_action<MoveLeftAction>(entity);
    GameUtils::emplace_action<MoveRightAction>(entity);
    GameUtils::emplace_action<FlashAction>(entity);
    for (int i = 1; i <= 10; i++) {
        GameUtils::emplace_action<SwitchSelectedItemAction>(entity, i);
    }
    GameUtils::emplace_action<CycleSelectedItemAction>(entity, true);
    GameUtils::emplace_action<CycleSelectedItemAction>(entity, false);
    GameUtils::emplace_action<SwitchInventoryAction>(entity);
    GameUtils::emplace_action<DropSelectedAction>(entity);

    ecs.add_components(entity, InventoryComponent{{entity}});
    auto& inventory = ecs.get_component<InventoryComponent>(entity).inventory;

    JsonListType obj = Json::parse_file(game_resource.path / "inventory.json");
    for (JsonDictType item : obj) {
        std::string name = item["name"];
        int count = item["count"];
        inventory.pick(name, count);
    }

    CollisionManager::instance().add(entity, true);
    SceneManager::instance().add(entity, 10);

    return entity;
}

void EntityManager::create_enemy(const std::string& name, Vector2D<float> position, bool random_elite) {
    if (position == Vector2D<float>{-1, -1}) {
        position = GameUtils::gen_spawn_boundary_position();
    }

    EntityTemplate t = template_map_[name];
    t[typeid(PositionComponent)] = PositionComponent{position};

    Entity entity = ecs.add_entity(t);

    auto& enemy_resource = ecs.get_resource<EnemyResource>();
    bool elite = random.uniform(0, 99) < enemy_resource.elite_chance;

    auto& hp = ecs.get_component<HPComponent>(entity);
    auto& velocity = ecs.get_component<VelocityComponent>(entity);
    auto& collide = ecs.get_component<CollideComponent>(entity);

    hp.max_hp = hp.hp = hp.max_hp * enemy_resource.max_hp_ratio / 100;
    velocity.max_speed = velocity.speed = velocity.speed * enemy_resource.max_speed_ratio / 100;
    collide.atk += enemy_resource.extra_collide_damage;

    if (elite) {
        hp.max_hp = hp.hp = hp.max_hp * enemy_resource.elite_max_hp_ratio / 100;
        collide.atk += enemy_resource.elite_extra_collide_damage;

        auto& [w, h] = ecs.get_component<SizeComponent>(entity).vec;
        w = w * enemy_resource.elite_size_ratio / 100;
        h = h * enemy_resource.elite_size_ratio / 100;
    }

    CollisionManager::instance().add(entity, true);
    SceneManager::instance().add(entity, 10);
    create_health_bar(entity);
}

void EntityManager::create_tower(const std::string& name, Entity master_entity, Vector2D<float> position) {
    int power = static_cast<const Tower::Data*>(ItemManager::instance().get_data(name))->power;
    EntityTemplate t = template_map_[name];
    t[typeid(PositionComponent)] = PositionComponent{position};
    t[typeid(MasterComponent)] = MasterComponent{master_entity};
    t[typeid(PerkComponent)] = PerkComponent{power};
    Entity entity = ecs.add_entity(t);

    ecs.add_components(entity, InventoryComponent{{entity}});
    auto& inventory = ecs.get_component<InventoryComponent>(entity).inventory;
    inventory.pick("usp");
    inventory.select(1);

    ecs.add_components(entity, AIComponent{BehaviorTreeManager::instance().get("tower")});

    // invincible now
    // CollisionManager::instance().add(entity, false);
    SceneManager::instance().add(entity, 9);
}

void EntityManager::create_structure(const std::string& name, Entity master_entity, Vector2D<float> position) {
    EntityTemplate t = template_map_[name];
    t[typeid(PositionComponent)] = PositionComponent{position};
    t[typeid(MasterComponent)] = MasterComponent{master_entity};
    Entity entity = ecs.add_entity(t);

    CollisionManager::instance().add(entity, false);
    SceneManager::instance().add(entity, 10);
}

void EntityManager::create_trap(const std::string& name, Entity master_entity, Vector2D<float> position) {
    EntityTemplate t = template_map_[name];
    t[typeid(PositionComponent)] = PositionComponent{position};
    t[typeid(MasterComponent)] = MasterComponent{master_entity};
    Entity entity = ecs.add_entity(t);

    CollisionManager::instance().add(entity, true);
    SceneManager::instance().add(entity, 1);
}

void EntityManager::create_floor(const std::string& name, Entity master_entity, Vector2D<float> position) {
    EntityTemplate t = template_map_[name];
    t[typeid(PositionComponent)] = PositionComponent{position};
    t[typeid(MasterComponent)] = MasterComponent{master_entity};
    Entity entity = ecs.add_entity(t);
    SceneManager::instance().add(entity, 0);
}

void EntityManager::create_bullet(const std::string& name, Vector2D<float> position, Vector2D<float> direction, int atk, int penetration, Entity master_entity) {
    EntityTemplate t = template_map_[name];

    t[typeid(PositionComponent)] = PositionComponent{std::move(position)};
    t[typeid(DirectionComponent)] = DirectionComponent{std::move(direction)};
    t[typeid(CollideComponent)] = CollideComponent{atk, penetration};
    t[typeid(MasterComponent)] = MasterComponent{master_entity};

    Entity entity = ecs.add_entity(t);

    CollisionManager::instance().add(entity, true);
    SceneManager::instance().add(entity, 11);
}

void EntityManager::create_item(const std::variant<std::string, std::shared_ptr<Item>>& data, Vector2D<float> position, int count, int unpickable_seconds) {
    EntityTemplate t;

    SDL_Texture* texture;
    std::visit([&](const auto& data) {
        if constexpr (std::is_same_v<std::decay_t<decltype(data)>, std::string>) {
            texture = TextureManager::instance().get_texture(data);
        } else if constexpr (std::is_same_v<std::decay_t<decltype(data)>, std::shared_ptr<Item>>) {
            texture = TextureManager::instance().get_texture(data->data().name);
        }
        t[typeid(TextureComponent)] = TextureComponent{texture};
    }, data);

    t[typeid(PositionComponent)] = PositionComponent{std::move(position)};
    auto [w, h] = sdl.get_texture_size(texture);
    t[typeid(SizeComponent)] = SizeComponent{{w, h}};

    Entity entity = ecs.add_entity(t);
    auto item = ItemComponent{data, count};
    if (unpickable_seconds) {
        timer_resource.add(config_resource.fps * 5, 1, [entity, item]() {
            ecs.add_components(entity, std::move(item));
        });
    } else {
        ecs.add_components(entity, std::move(item));
    }

    CollisionManager::instance().add(entity, true);
    SceneManager::instance().add(entity, 11);
}

void EntityManager::create_health_bar(Entity master) {
    EntityTemplate t = template_map_["health_bar"];
    t[typeid(MasterComponent)] = MasterComponent{master};
    const auto& size = ecs.get_component<SizeComponent>(master).vec;
    t[typeid(SizeComponent)] = SizeComponent{size.x, size.x / 8};
    const auto& pos = ecs.get_component<PositionComponent>(master).vec;
    t[typeid(PositionComponent)] = PositionComponent{};
    t[typeid(RelativePositionComponent)] = RelativePositionComponent{0, -size.y / 2};
    t[typeid(TextureComponent)] = TextureComponent{sdl.create_texture(size.x, size.x / 8, sdl.BLACK, SDL_TEXTUREACCESS_TARGET)};

    Entity entity = ecs.add_entity(t);

    SceneManager::instance().add(entity, 20, master);
}

Entity EntityManager::create_text(const std::string& text, SDL_Color color, const Vector2D<float>& pos) {
    auto texture = sdl.create_texture(text, color);
    auto [w, h] = sdl.get_texture_size(texture);
    Entity entity = ecs.add_entity(
        TextureComponent{texture},
        PositionComponent{pos},
        SizeComponent{{w, h}},
        VelocityComponent{10, 10}
    );

    SceneManager::instance().add(entity, 20);

    return entity;
}

void EntityManager::parse_enemy_json() {
    JsonListType obj = Json::parse_file(game_resource.path / "enemy.json");
    for (JsonDictType enemy : obj) {
        std::string name = enemy["name"];
        int hp = enemy["hp"];
        int velocity = enemy["velocity"];
        int collide = enemy["collide"];
        bool flyable = enemy.count("flyable") ? enemy["flyable"] : false;
        int exp = enemy["exp"];
        JsonListType reward_list = enemy["reward"];
        std::vector<RewardItem> rewards;
        for (JsonDictType reward : reward_list) {
            std::string reward_name = reward["name"];
            int count0 = reward["count"][0];
            int count1 = reward["count"][1];
            float chance = (double)reward["chance"];
            rewards.emplace_back(reward_name, std::pair<int, int>(count0, count1), chance);
        }

        EntityTemplate t;
        t[typeid(EnemyComponent)] = EnemyComponent{};
        t[typeid(PositionComponent)] = PositionComponent{};
        t[typeid(VelocityComponent)] = VelocityComponent{(float)velocity};
        t[typeid(DirectionComponent)] = DirectionComponent{};
        t[typeid(HPComponent)] = HPComponent{hp};
        t[typeid(CollideComponent)] = CollideComponent{collide, -1, 30};
        t[typeid(RewardComponent)] = RewardComponent{exp, rewards};
        std::string animation_name = enemy.count("animation_name") ? enemy["animation_name"] : name;
        t[typeid(AnimationComponent)] = AnimationComponent{animation_name};
        t[typeid(TextureComponent)] = TextureComponent{};
        if (!flyable) {
            t[typeid(ColliderComponent)] = ColliderComponent{};
            t[typeid(AStarTrackNearestPlayerTag)] = AStarTrackNearestPlayerTag{};
        } else {
            t[typeid(TrackNearestPlayerTag)] = TrackNearestPlayerTag{};
        }

        if (enemy.count("size")) {
            int size = enemy["size"];
            t[typeid(SizeComponent)] = SizeComponent{{(float)size, (float)size}};
        } else {
            float w, h;
            SDL_Texture* texture = AnimationManager::instance().get_animations(animation_name)->get_texture(0, 0);
            SDL_GetTextureSize(texture, &w, &h);
            t[typeid(SizeComponent)] = SizeComponent{{w, h}};
        }

        template_map_[name] = std::move(t);
    }
}

void EntityManager::cache_bullet(const std::string& name) {
    EntityTemplate t;
    t[typeid(SizeComponent)] = SizeComponent{{32, 32}};
    t[typeid(TextureComponent)] = TextureComponent{name};
    t[typeid(FriendComponent)] = FriendComponent{};
    t[typeid(VelocityComponent)] = VelocityComponent{300};
    t[typeid(BulletComponent)] = BulletComponent{};

    template_map_[name] = std::move(t);
}

void EntityManager::cache_tower(const std::string& name) {
    EntityTemplate t;
    t[typeid(AnimationComponent)] = AnimationComponent{name};
    t[typeid(TextureComponent)] = TextureComponent{};
    t[typeid(SizeComponent)] = SizeComponent{{48, 48}};
    t[typeid(TowerComponent)] = TowerComponent{300};
    t[typeid(ActionsComponent)] = ActionsComponent{};
    t[typeid(AimDirectionComponent)] = AimDirectionComponent{};

    template_map_[name] = std::move(t);
}

void EntityManager::cache_structure(const std::string& name) {
    EntityTemplate t;
    t[typeid(TextureComponent)] = TextureComponent{name};
    t[typeid(SizeComponent)] = SizeComponent{{48, 48}};

    template_map_[name] = std::move(t);
}

void EntityManager::cache_trap(const std::string& name) {
    auto data = static_cast<const Trap::Data*>(ItemManager::instance().get_data(name));

    EntityTemplate t;
    t[typeid(AnimationComponent)] = AnimationComponent{name};
    t[typeid(TextureComponent)] = TextureComponent{};
    t[typeid(FriendComponent)] = FriendComponent{};
    t[typeid(SizeComponent)] = SizeComponent{{48, 48}};
    t[typeid(TrapComponent)] = TrapComponent{};
    t[typeid(CollideComponent)] = CollideComponent{data->atk, data->durability, data->cooldown};

    template_map_[name] = std::move(t);
}

void EntityManager::cache_floor(const std::string& name) {
    EntityTemplate t;
    t[typeid(TextureComponent)] = TextureComponent{name};
    t[typeid(SizeComponent)] = SizeComponent{{48, 48}};

    template_map_[name] = std::move(t);
}

void EntityManager::cache_health_bar() {
    EntityTemplate t;
    t[typeid(HealthBarComponent)] = HealthBarComponent{};

    template_map_["health_bar"] = std::move(t);
}

}  // namespace wheel
