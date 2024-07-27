#include <game/entity_manager.hpp>

#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>

#include <wheel/json.hpp>

#include <game/global.hpp>
#include <game/utils.hpp>
#include <game/animation_manager.hpp>
#include <game/texture_manager.hpp>
#include <game/item_manager.hpp>

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

namespace wheel {

EntityManager::EntityManager() {
    parse_enemy_json();

    cache_bullet("bullet_shell");
    cache_tower("archer_tower");
}

Entity EntityManager::create_player(const std::string& name, bool self) {
    EntityTemplate t;
    t[typeid(PlayerComponent)] = PlayerComponent{};
    t[typeid(AnimationComponent)] = AnimationComponent{name};
    t[typeid(TextureComponent)] = TextureComponent{};
    t[typeid(PositionComponent)] = PositionComponent{{1920. / 2, 1080. / 2}};
    t[typeid(SizeComponent)] = SizeComponent{48, 48};
    t[typeid(VelocityComponent)] = VelocityComponent{100, 100};
    t[typeid(DirectionComponent)] = DirectionComponent{};
    t[typeid(HPComponent)] = HPComponent{3};
    t[typeid(LevelComponent)] = LevelComponent{1, 0};
    t[typeid(ActionsComponent)] = ActionsComponent{};
    t[typeid(MoveComponent)] = MoveComponent{};
    t[typeid(FriendComponent)] = FriendComponent{};
    t[typeid(InputComponent)] = InputComponent{};
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

    ecs.add_components(entity, InventoryComponent{{entity}});
    auto& inventory = ecs.get_component<InventoryComponent>(entity).inventory;
    inventory.pick("usp");
    inventory.pick("m3");
    inventory.pick("m4a1");
    inventory.pick("awp");
    inventory.pick("coin", 10);
    inventory.select(1);

    return entity;
}

void EntityManager::create_enemy(const std::string& name, Vector2D<double> position) {
    if (position == Vector2D<double>{-1, -1}) {
        position = GameUtils::gen_spawn_boundary_position();
    }

    EntityTemplate t = template_map_[name];
    t[typeid(PositionComponent)] = PositionComponent{position};

    Entity entity = ecs.add_entity(t);
    create_health_bar(entity);
}

void EntityManager::create_tower(const std::string& name, Entity master_entity, Vector2D<double> position) {
    if (position == Vector2D<double>{-1, -1}) {
        position = GameUtils::gen_spawn_internal_position(200);
    }

    EntityTemplate t = template_map_[name];
    t[typeid(PositionComponent)] = PositionComponent{position};
    t[typeid(MasterComponent)] = MasterComponent{master_entity};
    Entity entity = ecs.add_entity(t);

    ecs.add_components(entity, InventoryComponent{{entity}});
    auto& inventory = ecs.get_component<InventoryComponent>(entity).inventory;
    inventory.pick("usp");
    inventory.select(1);
}

void EntityManager::create_bullet(const std::string& name, Vector2D<double> position, Vector2D<double> direction, int atk, int penetration, Entity master_entity) {
    EntityTemplate t = template_map_[name];

    t[typeid(PositionComponent)] = PositionComponent{std::move(position)};
    t[typeid(DirectionComponent)] = DirectionComponent{std::move(direction)};
    t[typeid(CollideComponent)] = CollideComponent{atk, penetration};
    t[typeid(MasterComponent)] = MasterComponent{master_entity};

    ecs.add_entity(t);
}

void EntityManager::create_item(const std::string& name, Vector2D<double> position, int count) {
    EntityTemplate t;
    t[typeid(ItemComponent)] = ItemComponent{name, count};
    t[typeid(PositionComponent)] = PositionComponent{std::move(position)};
    SDL_Texture* texture = TextureManager::instance().get_texture(name);
    t[typeid(TextureComponent)] = TextureComponent{texture};
    auto [w, h] = sdl.get_texture_size(texture);
    t[typeid(SizeComponent)] = SizeComponent{(int)w, (int)h};

    ecs.add_entity(t);
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
        t[typeid(TrackNearestEnemyTag)] = TrackNearestEnemyTag{};
        std::string animation_name = enemy.count("animation_name") ? enemy["animation_name"] : name;
        t[typeid(AnimationComponent)] = AnimationComponent{animation_name};
        t[typeid(TextureComponent)] = TextureComponent{};

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
    t[typeid(DirectionComponent)] = DirectionComponent{};

    template_map_[name] = std::move(t);
}

}  // namespace wheel
