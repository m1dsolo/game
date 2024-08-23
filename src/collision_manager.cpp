#include <game/collision_manager.hpp>

#include <game/global.hpp>
#include <game/map.hpp>

#include <game/component/position.hpp>
#include <game/component/size.hpp>

namespace wheel {

CollisionManager::CollisionManager()
      : static_quadtree_(Map::instance().game_rect(), [](Entity entity)  {
          return Rect<float>{ecs.get_component<PositionComponent>(entity).vec, ecs.get_component<SizeComponent>(entity).vec};
      }), dynamic_quadtree_(Map::instance().game_rect(), [](Entity entity) {
          return Rect<float>{ecs.get_component<PositionComponent>(entity).vec, ecs.get_component<SizeComponent>(entity).vec / 2};
      }) {}

void CollisionManager::add(Entity entity, bool dynamic) {
    if (dynamic) {
        dynamic_quadtree_.add(entity);
    } else {
        static_quadtree_.add(entity);
    }
}

void CollisionManager::remove(Entity entity) {
    dynamic_quadtree_.remove(entity);
}

void CollisionManager::update() {
    dynamic_quadtree_.update();
}

std::vector<std::pair<Entity, Entity>> CollisionManager::find_all_dynamic_intersections() {
    return dynamic_quadtree_.find_all_intersections();
}

bool CollisionManager::is_collision_with_static(Entity entity) {
    const auto& position = ecs.get_component<PositionComponent>(entity).vec;
    const auto& size = ecs.get_component<SizeComponent>(entity).vec;
    return static_quadtree_.query(Rect<float>{position, size / 2}).size() > 0;
}

}  // namespace wheel
