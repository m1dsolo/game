#pragma once

#include <wheel/singleton.hpp>

namespace wheel {

class AudioSystem : public Singleton<AudioSystem> {
    friend class Singleton<AudioSystem>;

public:
    void startup();

private:
    AudioSystem() = default;
    ~AudioSystem() = default;
    AudioSystem(const AudioSystem&) = delete;
};

}  // namespace wheel
