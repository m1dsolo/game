#include <game/system/audio.hpp>

#include <game/global.hpp>
#include <game/audio_manager.hpp>

namespace wheel {

void AudioSystem::startup() {
    AudioManager::instance().play("game");
}

}  // namespace wheel
