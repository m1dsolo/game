#pragma once

#include <game/system/base.hpp>

namespace wheel {

class HandleGameEventSystem : public BaseSystem<HandleGameEventSystem> {
    friend class BaseSystem<HandleGameEventSystem>;

public:
    void execute_impl() override;

private:
    HandleGameEventSystem() : BaseSystem(true) {}
    ~HandleGameEventSystem() = default;
    HandleGameEventSystem(const HandleGameEventSystem&) = delete;
};

}  // namespace wheel

