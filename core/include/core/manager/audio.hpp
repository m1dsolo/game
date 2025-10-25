#pragma once

#include <wheel/singleton.hpp>
#include <wheel/id.hpp>
#include <sdl/sdl.hpp>

#include <vector>
#include <unordered_map>
#include <unordered_set>

namespace core {

class AudioManager : public wheel::Singleton<AudioManager> {
    friend class wheel::Singleton<AudioManager>;

public:
    void play(wheel::ID id);

    // stop/pause/resume only support music for now
    void stop(wheel::ID id);
    void pause(wheel::ID id);
    void resume(wheel::ID id);

private:
    AudioManager();
    ~AudioManager();
    AudioManager(const AudioManager&) = delete;

    struct SoundTrackCallbackData {
        std::unordered_set<size_t>* valid_sound_track_idxs_;
        size_t idx;
    };

    std::unordered_set<size_t> valid_sound_track_idxs_;
    std::vector<SoundTrackCallbackData> sound_track_callback_datas_;

    std::unordered_map<wheel::ID, MIX_Audio*> music_path2audio_;
    std::unordered_map<wheel::ID, MIX_Audio*> sound_path2audio_;

    MIX_Track* music_track_ = nullptr;
    std::vector<MIX_Track*> sound_tracks_;

    SDL_PropertiesID music_props_ = 0;
    SDL_PropertiesID sound_props_ = 0;

    MIX_Mixer* mixer_ = nullptr;

    static const int SOUND_TRACK_NUM = 16;
};

}  // namespace core
