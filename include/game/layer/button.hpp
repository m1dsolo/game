#pragma once

#include <string>
#include <functional>

#include <game/layer/layer.hpp>

namespace wheel {

class Button : public Layer {
public:
    Button(const std::string& text, int padding, std::function<void()> func);

    void on_attach() override;
    void on_detach() override;
    void on_update() override;
    void on_render() override;
    bool on_event(const SDL_Event& event) override;

    std::pair<int, int> size();
    SDL_FRect& dst() { return dst_; }

private:
    std::string text_;
    bool padding_;
    std::function<void()> func_;
    SDL_Texture* texture_ = nullptr;
    SDL_FRect dst_ {0., 0., 0., 0.};
    bool hover_ = false;
};

}  // namespace wheel

