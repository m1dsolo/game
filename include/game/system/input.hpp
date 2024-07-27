#pragma once

#include <SDL3/SDL.h>

#include <wheel/singleton.hpp>
#include <wheel/json.hpp>

namespace wheel {

class InputSystem : public Singleton<InputSystem> {
    friend class Singleton<InputSystem>;

public:
    void execute();

private:
    InputSystem();
    ~InputSystem() = default;
    InputSystem(const InputSystem&) = delete;
};

}  // namespace wheel
