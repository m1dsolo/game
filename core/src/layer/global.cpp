#include <core/layer/global.hpp>
#include <core/global.hpp>

#include <sdl/sdl.hpp>

namespace core {

bool GlobalLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_QUIT: context.running = false;
        case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
            if (event.window.windowID == SDL_GetWindowID(sdl::SDL::window())) {
                context.running = false;
            }
            break;
        }
        case SDL_EVENT_WINDOW_RESIZED: {
            if (event.window.windowID == SDL_GetWindowID(sdl::SDL::window())) {
                static const float virtual_ratio = static_cast<float>(config.virtual_window_width) / config.virtual_window_height;
                auto [window_width, window_height] = sdl::SDL::get_window_size();
                float current_ratio = static_cast<float>(window_width) / window_height;
                float scale;
                float x, y, w, h;
                if (current_ratio > virtual_ratio) {
                    scale = static_cast<float>(window_height) / config.virtual_window_height;
                    w = static_cast<int>(config.virtual_window_width * scale);
                    h = window_height;
                    x = (window_width - w) / 2;
                    y = 0;
                } else {
                    scale = static_cast<float>(window_width) / config.virtual_window_width;
                    w = window_width;
                    h = static_cast<int>(config.virtual_window_height * scale);
                    x = 0;
                    y = (window_height - h) / 2;
                }

                context.viewport = {x, y, w, h};
            }
            break;
        }
    }

    // final layer
    return true;
}

}  // namespace core
