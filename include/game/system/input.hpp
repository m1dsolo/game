#pragma once

#include <SDL3/SDL.h>

#include <game/system/base.hpp>
#include <wheel/json.hpp>

namespace wheel {

class InputSystem : public BaseSystem<InputSystem> {
    friend class BaseSystem<InputSystem>;

public:
    void execute_impl() override;

private:
    InputSystem() : BaseSystem(true) {}
    ~InputSystem() = default;
    InputSystem(const InputSystem&) = delete;
};

}  // namespace wheel
