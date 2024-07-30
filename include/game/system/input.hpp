#pragma once

#include <SDL3/SDL.h>

#include <wheel/json.hpp>

#include <game/system/base.hpp>
#include <game/ui.hpp>

namespace wheel {

class InputSystem : public BaseSystem<InputSystem> {
    friend class BaseSystem<InputSystem>;

public:
    void execute_impl() override;

private:
    InputSystem() : BaseSystem(true), ui_(UI::instance()) {}
    ~InputSystem() = default;
    InputSystem(const InputSystem&) = delete;

    UI& ui_;
};

}  // namespace wheel
