#include <game/system/event.hpp>

#include <game/global.hpp>
#include <game/ui.hpp>
#include <game/event.hpp>
#include <game/entity_manager.hpp>

#include <game/layer/card.hpp>

#include <game/component/tag.hpp>
#include <game/component/self.hpp>

namespace wheel {

void EventSystem::handle_sdl_event() {
    auto& ui = UI::instance();
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ui.handle_event(event);
    }
}

void EventSystem::startup() {
    // spawn event
    timer.add(1000000, -1, []() {
        int val = random.uniform(0, 3);
        if (val < 3) {
            EntityManager::instance().create_enemy("skeleton");
        } else {
            EntityManager::instance().create_enemy("goblin");
        }
    });
}

void EventSystem::handle_game_event() {
    // card
    for (auto& event : ecs.get_events<LevelUpEvent>()) {
        if (ecs.has_components<SelfComponent>(event.entity)) {
            UI::instance().push_back<CardLayer>();
        }
    }

    // pop layer
    for (auto& event : ecs.get_events<PopLayerEvent>()) {
        UI::instance().pop_back();
    }
}

void EventSystem::handle_tag() {
    for (auto [self, _] : ecs.get_components<SelfComponent, DelEntityTag>()) {
        game_resource.running = false;
        std::cout << std::format("you killed {}!", player_resource.killed) << std::endl;
    }
    for (auto [entity, _] : ecs.get_entity_and_components<DelEntityTag>()) {
        ecs.del_entity(entity);
    }
}

}  // namespace wheel
