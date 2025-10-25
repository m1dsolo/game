#include <core/manager/audio.hpp>

#include <iostream>

namespace core {

AudioManager::AudioManager() : sound_tracks_(SOUND_TRACK_NUM, nullptr) {
    SDL_AudioSpec spec{
        .format = SDL_AUDIO_S16,
        .channels = 2,
        .freq = 44100
    };
    mixer_ = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec);

    music_track_ = MIX_CreateTrack(mixer_);
    MIX_SetTrackGain(music_track_, 1.0f);
    valid_sound_track_idxs_.reserve(SOUND_TRACK_NUM);
    sound_track_callback_datas_.reserve(SOUND_TRACK_NUM);
    for (size_t i = 0; i < SOUND_TRACK_NUM; ++i) {
        valid_sound_track_idxs_.emplace(i);
        sound_track_callback_datas_.emplace_back(&valid_sound_track_idxs_, i);

        auto track = MIX_CreateTrack(mixer_);
        MIX_SetTrackGain(track, 0.8f);

        MIX_SetTrackStoppedCallback(track, [](void* userdata, MIX_Track* track) {
            auto* data = static_cast<SoundTrackCallbackData*>(userdata);
            data->valid_sound_track_idxs_->emplace(data->idx);
        }, &sound_track_callback_datas_[i]);

        sound_tracks_[i] = std::move(track);
    }

    music_props_ = SDL_CreateProperties();
    SDL_SetNumberProperty(music_props_, MIX_PROP_PLAY_LOOPS_NUMBER, -1);
    SDL_SetNumberProperty(music_props_, MIX_PROP_PLAY_FADE_IN_FRAMES_NUMBER, 44100 / 60 * 2);  // 2 seconds fade in

    sound_props_ = SDL_CreateProperties();

    for (const auto& dir : {"music", "sound"}) {
        auto root_path = fs::path("assets") / dir;
        for (const auto& entry : fs::recursive_directory_iterator(root_path)) {
            if (entry.is_regular_file()) {
                auto path = entry.path();
                if (path.extension() == ".wav" || path.extension() == ".ogg" || path.extension() == ".mp3") {
                    auto audio = MIX_LoadAudio(mixer_, path.c_str(), true);
                    std::filesystem::path p;
                    if (fs::is_symlink(path)) {
                        p = fs::relative(path, fs::read_symlink(root_path));
                    } else {
                        p = fs::relative(path, root_path);
                    }
                    std::string key = p.parent_path() / p.stem();
                    std::cout << "[begin load sound...]" << key << std::endl;

                    if (dir == std::string("music")) {
                        music_path2audio_[key] = audio;
                    } else {
                        sound_path2audio_[key] = audio;
                    }
                }
            }
        }
    }
}

// TODO: FIXME
AudioManager::~AudioManager() {
    // MIX_DestroyTrack(music_track_);
    // for (auto track : sound_tracks_) {
    //     MIX_DestroyTrack(track);
    // }
    // for (auto& [_, audio] : music_path2audio_) {
    //     MIX_DestroyAudio(audio);
    // }
    // for (auto& [_, audio] : sound_path2audio_) {
    //     MIX_DestroyAudio(audio);
    // }
    // MIX_DestroyMixer(mixer_);
}

void AudioManager::play(wheel::ID id) {
    if (auto iter = music_path2audio_.find(id); iter != music_path2audio_.end()) {
        MIX_SetTrackAudio(music_track_, iter->second);
        MIX_PlayTrack(music_track_, music_props_);
    }
    if (auto sound_iter = sound_path2audio_.find(id); sound_iter != sound_path2audio_.end()) {
        if (auto iter = valid_sound_track_idxs_.begin(); iter != valid_sound_track_idxs_.end()) {
            auto idx = *iter;
            valid_sound_track_idxs_.erase(idx);
            MIX_SetTrackAudio(sound_tracks_[idx], sound_iter->second);
            MIX_PlayTrack(sound_tracks_[idx], sound_props_);
        }
    }
}

void AudioManager::stop(wheel::ID id) {
    if (music_path2audio_.count(id)) {
        MIX_StopTrack(music_track_, 0.);
    }
}

void AudioManager::pause(wheel::ID id) {
    if (music_path2audio_.count(id)) {
        MIX_PauseTrack(music_track_);
    }
}

void AudioManager::resume(wheel::ID id) {
    if (music_path2audio_.count(id)) {
        MIX_ResumeTrack(music_track_);
    }
}

}  // namespace core
