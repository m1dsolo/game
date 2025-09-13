#include <core/game.hpp>
#include <core/global.hpp>

#include <wheel/log.hpp>
#include <sdl/sdl.hpp>

namespace core {

Game::Game() {
    // SDL
    wheel::Log::assert_(sdl::SDL::init(
        SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD,
        "game", config.virtual_window_width, config.virtual_window_height,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE
    ), SDL_GetError);
    wheel::Log::assert_(sdl::SDL::init_audio(), SDL_GetError);
    wheel::Log::assert_(sdl::SDL::init_ttf("assets/font/SauceCodeProNerdFont-Regular.ttf", 16), SDL_GetError);
    wheel::Log::assert_(sdl::SDL::init_gamepad(), SDL_GetError);

    // sdl::SDL::set_window_position(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    sdl::SDL::set_window_maximized();
    sdl::SDL::set_render_vsync(1);

    context.texture = sdl::SDL::create_texture(config.virtual_window_width, config.virtual_window_height);
}

void Game::run() {
    while (context.running) {
        sdl::SDL::set_render_target(context.texture);
        sdl::SDL::set_color(sdl::SDL::BLACK);
        sdl::SDL::render_clear();

        ecs.update();

        sdl::SDL::set_render_target(nullptr);
        sdl::SDL::set_color(sdl::SDL::BLACK);
        sdl::SDL::render_clear();

        sdl::SDL::render_texture(context.texture, nullptr, &context.viewport);

        sdl::SDL::render_present();
    }

    ecs.clear();
    sdl::SDL::destroy();
}

}  // namespace core
