#include <game/audio_manager.hpp>

#include <filesystem>
#include <format>

#include <wheel/log.hpp>

#include <game/global.hpp>

namespace fs = std::filesystem;

namespace wheel {

AudioManager::AudioManager() {
    // music
    const std::string& music_path = std::format("{}/resources/music", game_resource.path);
    for (const auto& entry : fs::directory_iterator(music_path)) {
        const std::string& name = entry.path().stem();
        load(name, entry.path());
    }

    // sound
    const std::string& sound_path = std::format("{}/resources/sound", game_resource.path);
    for (const auto& entry : fs::directory_iterator(sound_path)) {
        const std::string& name = entry.path().stem();
        load(name, entry.path());
    }
}

// TODO
AudioManager::~AudioManager() {
}

void AudioManager::play(const std::string& name, int frames) {
    if (sound_map_.count(name)) {
        auto sound = sound_map_[name];
        if (frames == -1) {
            Mix_PlayChannel(-1, sound, 0);
        } else {
            Mix_PlayChannelTimed(-1, sound, 0, frames * 1000 / config_resource.fps);
        }
    } else if (music_map_.count(name)) {
        auto music = music_map_[name];
        Mix_FadeInMusic(music, -1, 10000);
    }
}

void AudioManager::load(const std::string& name, const std::string& path) {
    Log::debug("SoundManager::load: sound_name: {}, file_name: {}", name, path);

    if (fs::is_directory(path)) {
        for (const auto& entry : fs::directory_iterator(path)) {
            load_file(name + "_" + entry.path().stem().string(), entry.path());
        }
    } else if (fs::is_regular_file(path)) {
        load_file(name, path);
    } else {
        Log::error("SoundManager::load: invalid file name: {}", path);
    }
}

void AudioManager::load_file(const std::string& name, const std::string& path) {
    if (path.substr(path.size() - 3, 3) == "wav") {
        Mix_Chunk* sound = Mix_LoadWAV(path.c_str());
        sound_map_[name] = sound;
    } else {
        Mix_Music* music = Mix_LoadMUS(path.c_str());
        music_map_[name] = music;
    }
}

}  // namespace wheel
