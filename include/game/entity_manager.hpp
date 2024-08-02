#pragma once

#include <string>
#include <unordered_map>

#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>

#include <game/global.hpp>

namespace wheel {

class EntityManager : public Singleton<EntityManager> {
    friend class Singleton<EntityManager>;

public:
    Entity create_player(const std::string& name, bool self = true);

    void create_enemy(const std::string& name, Vector2D<double> position = {-1, -1});

    void create_tower(const std::string& name, Entity master_entity, Vector2D<double> position);

    void create_structure(const std::string& name, Entity master_entity, Vector2D<double> position);

    void create_bullet(const std::string& name, Vector2D<double> position, Vector2D<double> direction, int atk, int penetration, Entity master_entity);

    void create_item(const std::string& name, Vector2D<double> position, int count = 1);

    Entity create_health_bar(Entity master);

    Entity create_text(const std::string& text, SDL_Color color, const Vector2D<double>& pos);

private:
    void parse_enemy_json();

    void cache_bullet(const std::string& name);
    void cache_tower(const std::string& name);
    void cache_structure(const std::string& name);

    EntityManager();
    ~EntityManager() = default;
    EntityManager(const EntityManager&) = delete;

    std::unordered_map<std::string, EntityTemplate> template_map_;
};

}  // namespace wheel
