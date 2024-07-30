#pragma once

#include <game/system/base.hpp>

namespace wheel {

class AudioSystem : public BaseSystem<AudioSystem> {
    friend class BaseSystem<AudioSystem>;

public:
    void startup();

private:
    AudioSystem() : BaseSystem(true) {}
    ~AudioSystem() = default;
    AudioSystem(const AudioSystem&) = delete;
};

}  // namespace wheel
