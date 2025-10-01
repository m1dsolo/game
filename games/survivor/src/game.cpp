#include <survivor/game.hpp>
#include <core/manager/game.hpp>
#include <core/manager/layer.hpp>
#include <core/manager/audio.hpp>
#include <core/system/track.hpp>
#include <core/system/move.hpp>
#include <core/system/collider.hpp>
#include <survivor/system/pick_item.hpp>
#include <survivor/system/range_attack.hpp>
#include <survivor/system/aura_damage.hpp>
#include <survivor/system/hp_change.hpp>
#include <survivor/system/level.hpp>
#include <survivor/layer/game.hpp>
#include <survivor/global.hpp>

#include <wheel/log.hpp>

#include <rfl/json.hpp>

using namespace core;

namespace survivor {

SurvivorGame::SurvivorGame() {
    std::ifstream file("assets/config/game.json");
    if (file.is_open()) {
        game_config = rfl::json::read<GameConfig>(file).value();
    } else {
        wheel::Log::error("Can't find game.json!");
    }

    GameManager::instance().add_game_systems<
        TrackSystem,
        MoveSystem,
        ColliderSystem,
        RangeAttackSystem,
        AuraDamageSystem,
        HPChangeSystem,
        PickItemSystem,
        LevelSystem
    >();

    AudioManager::instance().play("bgm.mp3");
    LayerManager::instance().register_layer<GameLayer>();
}

}  // namespace survivor
