#include <survivor/game.hpp>
#include <core/manager/game.hpp>
#include <core/manager/layer.hpp>
#include <core/manager/audio.hpp>
#include <core/system/track.hpp>
#include <core/system/move.hpp>
#include <core/system/collider.hpp>
#include <survivor/system/range_attack.hpp>
#include <survivor/system/aura_damage.hpp>
#include <survivor/system/hp_change.hpp>
#include <survivor/layer/game.hpp>

using namespace core;

namespace survivor {

SurvivorGame::SurvivorGame() {
    GameManager::instance().add_game_systems<
        TrackSystem,
        MoveSystem,
        ColliderSystem,
        RangeAttackSystem,
        AuraDamageSystem,
        HPChangeSystem
    >();

    AudioManager::instance().play("bgm.mp3");
    LayerManager::instance().register_layer<GameLayer>();
}

}  // namespace survivor
