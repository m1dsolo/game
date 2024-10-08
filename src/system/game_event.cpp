#include <game/system/game_event.hpp>

#include <game/global.hpp>
#include <game/ui.hpp>
#include <game/event.hpp>
#include <game/entity_manager.hpp>
#include <game/layer_manager.hpp>
#include <game/collision_manager.hpp>
#include <game/scene_manager.hpp>

#include <game/layer/card.hpp>
#include <game/layer/cursor.hpp>

#include <game/component/tag.hpp>
#include <game/component/self.hpp>

namespace wheel {

void GameEventSystem::execute_impl() {
    // card
    for (auto& event : ecs.get_events<LevelUpEvent>()) {
        if (ecs.has_components<SelfComponent>(event.entity)) {
            UI::instance().push_back(LayerManager::instance().get("player_cards"));
        }
    }

    // pop layer
    for (auto& event : ecs.get_events<PopLayerEvent>()) {
        UI::instance().pop_back();
    }

    // enemy dead
    for (auto& event : ecs.get_events<EnemyDeadEvent>()) {
        GameManager::instance().enemy_cnt()--;
    }

    // game over
    for (auto [self, _] : ecs.get_components<SelfComponent, DelEntityTag>()) {
        GameManager::instance().quit();
        std::cout << std::format("you killed {}!", player_resource.killed) << std::endl;
    }

    // del entity
    auto& collision_manager = CollisionManager::instance();
    auto& scene_manager = SceneManager::instance();
    for (auto [entity, _] : ecs.get_entity_and_components<DelEntityTag>()) {
        collision_manager.remove(entity);
        scene_manager.remove(entity);
        ecs.del_entity(entity);
    }
}

}  // namespace wheel
