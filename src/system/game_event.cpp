#include <game/system/game_event.hpp>

#include <game/global.hpp>
#include <game/ui.hpp>
#include <game/event.hpp>
#include <game/entity_manager.hpp>

#include <game/layer/card.hpp>
#include <game/layer/cursor.hpp>

#include <game/component/tag.hpp>
#include <game/component/self.hpp>

namespace wheel {

void GameEventSystem::execute_impl() {
    // card
    for (auto& event : ecs.get_events<LevelUpEvent>()) {
        if (ecs.has_components<SelfComponent>(event.entity)) {
            auto& ui = UI::instance();
            auto& layers = ui.layers();
            auto back_layer = layers.back();
            if (dynamic_cast<CursorLayer*>(back_layer)) {
                ui.pop_back();
                ui.push_back<CardLayer>();
                layers.emplace_back(back_layer);
            } else {
                ui.push_back<CardLayer>();
            }
        }
    }

    // pop layer
    for (auto& event : ecs.get_events<PopLayerEvent>()) {
        UI::instance().pop_back();
    }

    // game over
    for (auto [self, _] : ecs.get_components<SelfComponent, DelEntityTag>()) {
        GameManager::instance().quit();
        std::cout << std::format("you killed {}!", player_resource.killed) << std::endl;
    }

    // del entity
    for (auto [entity, _] : ecs.get_entity_and_components<DelEntityTag>()) {
        ecs.del_entity(entity);
    }
}

}  // namespace wheel
