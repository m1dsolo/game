#pragma once

#include <string>
#include <unordered_map>

#include <wheel/singleton.hpp>

#include <SDL3_mixer/SDL_mixer.h>

namespace wheel {

class AudioManager final : public Singleton<AudioManager> {
    friend class Singleton<AudioManager>;

public:
    void play(const std::string& name, int frames = -1);

private:
    AudioManager();
    ~AudioManager();
    AudioManager(const AudioManager&) = delete;

    void load(const std::string& name, const std::string& path);
    void load_file(const std::string& name, const std::string& path);

    std::unordered_map<std::string, Mix_Chunk*> sound_map_;
    std::unordered_map<std::string, Mix_Music*> music_map_;
};

}  // namespace wheel
