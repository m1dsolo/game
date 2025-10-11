#include <survivor/game.hpp>
#include <survivor/layer/game.hpp>
#include <survivor/system/achievement.hpp>
#include <survivor/system/update_hud.hpp>

#include <core/global.hpp>
#include <core/manager/system.hpp>
#include <core/manager/audio.hpp>
#include <core/manager/layer.hpp>
#include <core/system/timer.hpp>
#include <core/system/track.hpp>
#include <core/system/move.hpp>
#include <core/system/collider.hpp>
#include <core/system/trigger.hpp>
#include <core/system/attack.hpp>
#include <core/system/range_attack.hpp>
#include <core/system/aura_damage.hpp>
#include <core/system/hp_change.hpp>
#include <core/system/generate_hp_float_text.hpp>
#include <core/system/death.hpp>
#include <core/system/drop_item.hpp>
#include <core/system/pick_item.hpp>
#include <core/system/level.hpp>

using namespace core;

namespace survivor {

SurvivorGame::SurvivorGame() {
    SystemManager::instance().add_game_systems<
        TimerSystem,
        TrackSystem,
        MoveSystem,
        ColliderSystem,
        TriggerSystem,
        AttackSystem,
        RangeAttackSystem,
        AuraDamageSystem,
        HPChangeSystem,
        GenerateHPFloatTextSystem,
        DeathSystem,
        AchievementSystem,
        UpdateHudSystem,
        DropItemSystem,
        PickItemSystem,
        LevelSystem
    >();

    AudioManager::instance().play("bgm.mp3");
    LayerManager::instance().register_layer<GameLayer>();
}

}  // namespace survivor
