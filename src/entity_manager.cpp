#include <game/entity_manager.hpp>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

#include <wheel/json.hpp>

#include <game/global.hpp>
#include <game/utils.hpp>
#include <game/animation_manager.hpp>
#include <game/texture_manager.hpp>
#include <game/item_manager.hpp>
#include <game/map.hpp>

#include <game/item/tower.hpp>
#include <game/item/structure.hpp>

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

#include <game/resource/enemy.hpp>

namespace wheel {

EntityManager::EntityManager() {
    parse_enemy_json();

    cache_bullet("bullet_shell");
    cache_tower("archer_tower");
    cache_structure("floor");
    cache_structure("wall");
    cache_structure("door");
}

Entity EntityManager::create_player(const std::string& name, bool self) {
    auto [x0, y0, x1, y1] = Map::instance().game_rect();

    EntityTemplate t;
    t[typeid(PlayerComponent)] = PlayerComponent{};
    t[typeid(PerkComponent)] = PerkComponent{};
    t[typeid(AnimationComponent)] = AnimationComponent{name};
    t[typeid(TextureComponent)] = TextureComponent{};
    t[typeid(PositionComponent)] = PositionComponent{(x0 + x1) / 2., (y0 + y1) / 2.};
    t[typeid(SizeComponent)] = SizeComponent{48, 48};
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
    inventory.pick("usp");
    inventory.pick("m3");
    inventory.pick("m4a1");
    inventory.pick("awp");
    inventory.pick("hp_potion", 1);
    inventory.pick("archer_tower", 1);
    inventory.pick("floor", 10);
    inventory.pick("wall", 50);
    inventory.pick("door", 10);
    inventory.pick("coin", 10);
    inventory.pick("usp");
    inventory.pick("m3");
    inventory.pick("m4a1");
    inventory.pick("awp");
    inventory.select(1);

    return entity;
}

void EntityManager::create_enemy(const std::string& name, Vector2D<double> position, bool random_elite) {
    if (position == Vector2D<double>{-1, -1}) {
        position = GameUtils::gen_spawn_boundary_position();
    }

    EntityTemplate t = template_map_[name];
    t[typeid(PositionComponent)] = PositionComponent{position};

    Entity entity = ecs.add_entity(t);
    create_health_bar(entity);


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

        auto& size = ecs.get_component<SizeComponent>(entity);
        size.w = size.w * enemy_resource.elite_size_ratio / 100;
        size.h = size.h * enemy_resource.elite_size_ratio / 100;
    }
}

void EntityManager::create_tower(const std::string& name, Entity master_entity, Vector2D<double> position) {
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
}

void EntityManager::create_structure(const std::string& name, Entity master_entity, Vector2D<double> position) {
    EntityTemplate t = template_map_[name];
    t[typeid(PositionComponent)] = PositionComponent{position};
    t[typeid(MasterComponent)] = MasterComponent{master_entity};
    Entity entity = ecs.add_entity(t);
}

void EntityManager::create_bullet(const std::string& name, Vector2D<double> position, Vector2D<double> direction, int atk, int penetration, Entity master_entity) {
    EntityTemplate t = template_map_[name];

    t[typeid(PositionComponent)] = PositionComponent{std::move(position)};
    t[typeid(DirectionComponent)] = DirectionComponent{std::move(direction)};
    t[typeid(CollideComponent)] = CollideComponent{atk, penetration};
    t[typeid(MasterComponent)] = MasterComponent{master_entity};

    ecs.add_entity(t);
}

void EntityManager::create_item(const std::variant<std::string, std::shared_ptr<Item>>& data, Vector2D<double> position, int count, int unpickable_seconds) {
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
    t[typeid(SizeComponent)] = SizeComponent{(int)w, (int)h};

    Entity entity = ecs.add_entity(t);
    auto item = ItemComponent{data, count};
    if (unpickable_seconds) {
        timer_resource.add(config_resource.fps * 5, 1, [entity, item]() {
            ecs.add_components(entity, std::move(item));
        });
    } else {
        ecs.add_components(entity, std::move(item));
    }
}

Entity EntityManager::create_health_bar(Entity master) {
    auto size = ecs.get_component<SizeComponent>(master);
    size.h = size.w / 8;
    return ecs.add_entity(
        HealthBarComponent{master},
        PositionComponent{},
        size
    );
}

Entity EntityManager::create_text(const std::string& text, SDL_Color color, const Vector2D<double>& pos) {
    auto texture = sdl.create_texture(text, color);
    auto [w, h] = sdl.get_texture_size(texture);
    return ecs.add_entity(
        TextureComponent{texture},
        PositionComponent{pos},
        SizeComponent{(int)w, (int)h},
        VelocityComponent{10, 10}
    );
}

void EntityManager::parse_enemy_json() {
    JsonListType obj = Json::parse_file(game_resource.path + "/enemy.json");
    for (JsonDictType enemy : obj) {
        std::string name = enemy["name"];
        int hp = enemy["hp"];
        int velocity = enemy["velocity"];
        int collide = enemy["collide"];
        int exp = enemy["exp"];
        JsonListType reward_list = enemy["reward"];
        std::vector<RewardItem> rewards;
        for (JsonDictType reward : reward_list) {
            std::string reward_name = reward["name"];
            int count0 = reward["count"][0];
            int count1 = reward["count"][1];
            double chance = reward["chance"];
            rewards.emplace_back(reward_name, std::pair<int, int>(count0, count1), chance);
        }

        EntityTemplate t;
        t[typeid(EnemyComponent)] = EnemyComponent{};
        t[typeid(PositionComponent)] = PositionComponent{};
        t[typeid(VelocityComponent)] = VelocityComponent{(double)velocity};
        t[typeid(DirectionComponent)] = DirectionComponent{};
        t[typeid(HPComponent)] = HPComponent{hp};
        t[typeid(CollideComponent)] = CollideComponent{collide, -1, 30};
        t[typeid(RewardComponent)] = RewardComponent{exp, rewards};
        // t[typeid(TrackNearestPlayerTag)] = TrackNearestPlayerTag{};
        t[typeid(AStarTrackNearestPlayerTag)] = AStarTrackNearestPlayerTag{};
        std::string animation_name = enemy.count("animation_name") ? enemy["animation_name"] : name;
        t[typeid(AnimationComponent)] = AnimationComponent{animation_name};
        t[typeid(TextureComponent)] = TextureComponent{};
        t[typeid(ColliderComponent)] = ColliderComponent{};

        if (enemy.count("size")) {
            int size = enemy["size"];
            t[typeid(SizeComponent)] = SizeComponent{size, size};
        } else {
            float w, h;
            SDL_Texture* texture = AnimationManager::instance().get_animations(animation_name)->get_texture(0, 0, false);
            SDL_GetTextureSize(texture, &w, &h);
            t[typeid(SizeComponent)] = SizeComponent{(int)w, (int)h};
        }

        template_map_[name] = std::move(t);
    }
}

void EntityManager::cache_bullet(const std::string& name) {
    EntityTemplate t;
    t[typeid(SizeComponent)] = SizeComponent{32, 32};
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
    t[typeid(SizeComponent)] = SizeComponent{48, 48};
    t[typeid(FriendComponent)] = FriendComponent{};
    t[typeid(TowerComponent)] = TowerComponent{300};
    t[typeid(ActionsComponent)] = ActionsComponent{};
    t[typeid(AimDirectionComponent)] = AimDirectionComponent{};

    template_map_[name] = std::move(t);
}

void EntityManager::cache_structure(const std::string& name) {
    EntityTemplate t;
    t[typeid(TextureComponent)] = TextureComponent{name};
    t[typeid(SizeComponent)] = SizeComponent{48, 48};
    t[typeid(FriendComponent)] = FriendComponent{};

    template_map_[name] = std::move(t);
}

}  // namespace wheel
