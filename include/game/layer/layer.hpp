#pragma once

#include <SDL3/SDL.h>

namespace wheel {

class Layer {
public:
    virtual ~Layer() = default;

    virtual void on_attach() {}
    virtual void on_detach() {}
    virtual void on_update() {}
    virtual void on_render() {}
    virtual bool on_event(const SDL_Event& event) { return false; }

protected:
    Layer() = default;
};

}  // namespace wheel
