#pragma once

#include <game/system/base.hpp>

namespace wheel {

class GameEventSystem : public BaseSystem<GameEventSystem> {
    friend class BaseSystem<GameEventSystem>;

public:
    void execute_impl() override;

private:
    GameEventSystem() : BaseSystem(true) {}
    ~GameEventSystem() = default;
    GameEventSystem(const GameEventSystem&) = delete;
};

}  // namespace wheel

