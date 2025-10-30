#pragma once

union SDL_Event;

namespace core {

class Layer {
public:
    virtual ~Layer() = default;

    virtual void on_register() {}
    virtual void on_attach() {}
    virtual void on_detach() {}
    virtual void on_show() {}
    virtual void on_hide() {}
    virtual void on_update() {}
    virtual bool on_event(const SDL_Event& event) { return false; }

protected:
    Layer() {}
};

}  // namespace core
