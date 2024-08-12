#pragma once

#include <string>
#include <variant>
#include <unordered_map>

#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>

#include <game/global.hpp>
#include <game/item/item.hpp>

namespace wheel {

class EntityManager : public Singleton<EntityManager> {
    friend class Singleton<EntityManager>;

public:
    Entity create_player(const std::string& name, bool self = true);
    void create_enemy(const std::string& name, Vector2D<float> position = {-1, -1}, bool random_elite = true);
    void create_tower(const std::string& name, Entity master_entity, Vector2D<float> position);
    void create_structure(const std::string& name, Entity master_entity, Vector2D<float> position);
    void create_trap(const std::string& name, Entity master_entity, Vector2D<float> position);
    void create_floor(const std::string& name, Entity master_entity, Vector2D<float> position);
    void create_bullet(const std::string& name, Vector2D<float> position, Vector2D<float> direction, int atk, int penetration, Entity master_entity);
    void create_item(const std::variant<std::string, std::shared_ptr<Item>>& data, Vector2D<float> position, int count = 1, int unpickable_seconds = 0);
    Entity create_health_bar(Entity master);
    Entity create_text(const std::string& text, SDL_Color color, const Vector2D<float>& pos);

private:
    void parse_enemy_json();

    void cache_bullet(const std::string& name);
    void cache_tower(const std::string& name);
    void cache_structure(const std::string& name);
    void cache_trap(const std::string& name);
    void cache_floor(const std::string& name);

    EntityManager();
    ~EntityManager() = default;
    EntityManager(const EntityManager&) = delete;

    std::unordered_map<std::string, EntityTemplate> template_map_;
};

}  // namespace wheel
