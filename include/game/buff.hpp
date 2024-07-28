#pragma once

#include <game/global.hpp>

namespace wheel {

class Buff {
public:
    Buff() {}
    virtual ~Buff() = default;
    
    virtual void on_effect(Entity entity = EntityNone) = 0;
};

class HPBuff : public Buff {
public:
    HPBuff(int hp) : hp_(hp) {}

    void on_effect(Entity entity = EntityNone) override;

private:
    int hp_;
};

}  // namespace wheel
