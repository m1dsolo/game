#include <survivor/game.hpp>
#include <survivor/system/achievement.hpp>
#include <survivor/layer/game.hpp>

#include <core/global.hpp>
#include <core/manager/game.hpp>
#include <core/manager/layer.hpp>
#include <core/manager/audio.hpp>
#include <core/system/track.hpp>
#include <core/system/move.hpp>
#include <core/system/collider.hpp>
#include <core/system/range_attack.hpp>
#include <core/system/aura_damage.hpp>
#include <core/system/hp_change.hpp>
#include <core/system/generate_hp_float_text.hpp>
#include <core/system/death.hpp>
#include <core/system/drop_item.hpp>
#include <core/system/pick_item.hpp>
#include <core/system/level.hpp>

#include <wheel/log.hpp>

using namespace core;

namespace survivor {

SurvivorGame::SurvivorGame() {
    GameManager::instance().add_game_systems<
        TrackSystem,
        MoveSystem,
        ColliderSystem,
        RangeAttackSystem,
        AuraDamageSystem,
        HPChangeSystem,
        GenerateHPFloatTextSystem,
        DeathSystem,
        AchievementSystem,
        DropItemSystem,
        PickItemSystem,
        LevelSystem
    >();

    AudioManager::instance().play("bgm.mp3");
    LayerManager::instance().register_layer<GameLayer>();
}

}  // namespace survivor
