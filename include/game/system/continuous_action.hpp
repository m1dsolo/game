#pragma once

#include <SDL3/SDL.h>

#include <game/system/base.hpp>

namespace wheel {

class ContinuousActionSystem : public BaseSystem<ContinuousActionSystem> {
    friend class BaseSystem<ContinuousActionSystem>;

public:
    void execute_impl() override;

private:
    ContinuousActionSystem() : BaseSystem(false) {}
    ~ContinuousActionSystem() = default;
    ContinuousActionSystem(const ContinuousActionSystem&) = delete;
};

}  // namespace wheel
