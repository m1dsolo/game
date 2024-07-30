#include <game/game_manager.hpp>

#include <wheel/log.hpp>

#include <game/global.hpp>
#include <game/entity_manager.hpp>
#include <game/client.hpp>
#include <game/ui.hpp>

#include <game/layer/map.hpp>
#include <game/layer/game.hpp>
#include <game/layer/status.hpp>
#include <game/layer/card.hpp>
#include <game/layer/hotbar.hpp>

#include <game/system/input.hpp>
#include <game/system/audio.hpp>
#include <game/system/event.hpp>
#include <game/system/move.hpp>
#include <game/system/animation.hpp>
#include <game/system/collide.hpp>
#include <game/system/timer.hpp>
#include <game/system/combat.hpp>
#include <game/system/tower.hpp>

namespace wheel {

GameManager::GameManager() {
}

GameManager::~GameManager() {
    ecs.shutdown();
}

void GameManager::run() {
    if (config_resource.online) {
        Client::instance().connect();
    }
    ecs.add_startup_system(std::bind(&AudioSystem::startup, &AudioSystem::instance()));
    ecs.add_startup_system(std::bind(&EventSystem::startup, &EventSystem::instance()));

    ecs.add_system(std::bind(&EventSystem::handle_sdl_event, &EventSystem::instance()));
    ecs.add_system(std::bind(&InputSystem::execute, &InputSystem::instance()));
    ecs.add_system(std::bind(&MoveSystem::execute, &MoveSystem::instance()));  // need event
    ecs.add_system(std::bind(&TowerSystem::execute, &TowerSystem::instance()));
    ecs.add_system(std::bind(&CollideSystem::execute, &CollideSystem::instance()));
    ecs.add_system(std::bind(&AnimationSystem::execute, &AnimationSystem::instance()));  // need move
    ecs.add_system(std::bind(&CombatSystem::execute, &CombatSystem::instance()));
    ecs.add_system(std::bind(&UI::update, &UI::instance()));
    ecs.add_system(std::bind(&UI::render, &UI::instance()));  // need animation
    ecs.add_system(std::bind(&EventSystem::handle_game_event, &EventSystem::instance()));
    ecs.add_system(std::bind(&EventSystem::handle_tag, &EventSystem::instance()));
    ecs.add_system(std::bind(&TimerSystem::execute, &TimerSystem::instance()));  // at last

    if (!config_resource.online) {
        auto& entity_mangaer = EntityManager::instance();
        Entity entity = EntityManager::instance().create_player("slime", true);
        // EntityManager::instance().create_enemy("skeleton", {800., 600.});
        // EntityManager::instance().create_tower("archer_tower", entity);
    }

    auto& ui = UI::instance();
    ui.push_back<MapLayer>();  // use SelfComponent
    ui.push_back<GameLayer>();
    ui.push_back<StatusLayer>();
    ui.push_back<HotBarLayer>();

    ecs.startup();

    while (game_resource.running) {
        ecs.update();
    }
}

}  // namespace wheel
