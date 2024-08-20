#include <game/game_manager.hpp>

#include <wheel/log.hpp>

#include <game/global.hpp>
#include <game/entity_manager.hpp>
#include <game/ui.hpp>
#include <game/event.hpp>
#include <game/camera.hpp>
#include <game/layer_manager.hpp>

#include <game/layer/map.hpp>
#include <game/layer/game.hpp>
#include <game/layer/status.hpp>
#include <game/layer/card.hpp>
#include <game/layer/construction_stage.hpp>
#include <game/layer/cursor.hpp>

#include <game/system/audio.hpp>
#include <game/system/input.hpp>
#include <game/system/continuous_action.hpp>
#include <game/system/move.hpp>
#include <game/system/camera.hpp>
#include <game/system/tower.hpp>
#include <game/system/collide.hpp>
#include <game/system/animation.hpp>
#include <game/system/combat.hpp>
#include <game/system/render.hpp>
#include <game/system/game_event.hpp>
#include <game/system/timer.hpp>

#include <game/component/self.hpp>
#include <game/component/inventory.hpp>
#include <game/component/item.hpp>
#include <game/component/tag.hpp>
#include <game/component/ai.hpp>
#include <game/component/direction.hpp>

#include <game/resource/enemy.hpp>

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
    auto& entity_mangaer = EntityManager::instance();
    Entity entity = EntityManager::instance().create_player("slime", true);

    ecs.add_resource<EnemyResource>();

    ecs.add_startup_system(std::bind(&AudioSystem::startup, &AudioSystem::instance()));

    add_system<InputSystem>();
    add_system<ContinuousActionSystem>();
    add_system<MoveSystem>();
    add_system<CameraSystem>();
    add_system<TowerSystem>();
    add_system<CollideSystem>();
    add_system<AnimationSystem>();
    add_system<CombatSystem>();
    add_system<RenderSystem>();
    add_system<GameEventSystem>();
    add_system<TimerSystem>();

    auto& ui = UI::instance();
    ui.push_back<MapLayer>();  // use SelfComponent
    ui.push_back<GameLayer>();
    ui.push_back<StatusLayer>();
    ui.push_back(LayerManager::instance().get("hotbar"));
    ui.push_back<ConstructionStageLayer>();

    ecs.startup();

    while (running_) {
        ecs.update();

        // finish combat
        if (combat_time_over_ && !enemy_cnt_) {
            combat_time_over_ = false;
            swap_stage();
        }
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
    // construction to combat
    if (!stage_) {
        // add enemy card
        CardLayer* layer = static_cast<CardLayer*>(LayerManager::instance().get("enemy_cards"));
        layer->set_detach_callback([&]() {
            UI::instance().del<ConstructionStageLayer>();

            auto& enemy_resource = ecs.get_resource<EnemyResource>();
            int combat_seconds = enemy_resource.combat_seconds;
            int enemy_num = enemy_resource.enemy_num;
            enemy_cnt_ = enemy_num;
            timer_resource.add(config_resource.fps * combat_seconds / enemy_num, enemy_num, [this](int cnt) {
                int val = random.uniform(1, 4);
                if (val <= 2) {
                    EntityManager::instance().create_enemy("skeleton");
                } else if (val <= 3){
                    EntityManager::instance().create_enemy("goblin");
                } else if (val <= 4) {
                    EntityManager::instance().create_enemy("skull");
                }

                if (cnt == 1) {
                    combat_time_over_ = true;
                }
            }, true);
        });
        UI::instance().push_back(layer);
    }
    // combat to construction
    else {
        UI::instance().push_back<ConstructionStageLayer>();

        // pick all item(TODO: if multiplayer)
        Entity entity = ecs.get_entities<SelfComponent>()[0];
        auto& inventory = ecs.get_component<InventoryComponent>(entity).inventory;
        for (auto [entity, item] : ecs.get_entity_and_components<ItemComponent>()) {
            ecs.add_components(
                entity,
                TrackNearestPlayerTag{},
                DirectionComponent{},
                VelocityComponent{500}
            );
        }
    }
    stage_ = !stage_;
}

}  // namespace wheel
