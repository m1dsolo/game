#include <game/game_manager.hpp>

#include <wheel/log.hpp>

#include <game/global.hpp>
#include <game/entity_manager.hpp>
#include <game/ui.hpp>

#include <game/layer/map.hpp>
#include <game/layer/game.hpp>
#include <game/layer/status.hpp>
#include <game/layer/card.hpp>
#include <game/layer/hotbar.hpp>

#include <game/system/audio.hpp>
#include <game/system/event.hpp>
#include <game/system/input.hpp>
#include <game/system/continuous_action.hpp>
#include <game/system/move.hpp>
#include <game/system/tower.hpp>
#include <game/system/collide.hpp>
#include <game/system/animation.hpp>
#include <game/system/combat.hpp>
#include <game/system/render.hpp>
#include <game/system/game_event.hpp>
#include <game/system/timer.hpp>

namespace wheel {

GameManager::GameManager() {
}

GameManager::~GameManager() {
    ecs.shutdown();
}

template <typename T> requires std::derived_from<T, BaseSystem<T>>
void add_system() {
    ecs.add_system(std::bind(&T::execute, &T::instance()));
}

void GameManager::run() {
    ecs.add_startup_system(std::bind(&AudioSystem::startup, &AudioSystem::instance()));
    ecs.add_startup_system(std::bind(&EventSystem::startup, &EventSystem::instance()));

    add_system<InputSystem>();
    add_system<ContinuousActionSystem>();
    add_system<MoveSystem>();
    add_system<TowerSystem>();
    add_system<CollideSystem>();
    add_system<AnimationSystem>();
    add_system<CombatSystem>();
    add_system<RenderSystem>();
    add_system<GameEventSystem>();
    add_system<TimerSystem>();

    auto& entity_mangaer = EntityManager::instance();
    Entity entity = EntityManager::instance().create_player("slime", true);
    // EntityManager::instance().create_enemy("skeleton", {800., 600.});
    // EntityManager::instance().create_tower("archer_tower", entity);

    auto& ui = UI::instance();
    ui.push_back<MapLayer>();  // use SelfComponent
    ui.push_back<GameLayer>();
    ui.push_back<StatusLayer>();
    ui.push_back<HotBarLayer>();

    ecs.startup();

    while (running_) {
        ecs.update();
    }
}

void GameManager::quit() {
    running_ = false;
}

void GameManager::pause() {
    if (paused_) {
        return;
    }
    paused_ = true;
    timer.pause();
}

void GameManager::resume() {
    if (!paused_) {
        return;
    }
    paused_ = false;
    timer.resume();
}

void GameManager::swap_stage() {
}

}  // namespace wheel
