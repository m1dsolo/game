#include <core/game.hpp>
#include <core/global.hpp>

#include <wheel/log.hpp>
#include <sdl/sdl.hpp>

namespace core {

Game::Game() {
    // SDL
    wheel::Log::assert_(sdl::SDL::init(
        SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD,
        "game", config.window_width, config.window_height,
        SDL_WINDOW_RESIZABLE
    ), SDL_GetError);
    wheel::Log::assert_(sdl::SDL::init_audio(), SDL_GetError);
    wheel::Log::assert_(sdl::SDL::init_ttf("assets/font/SauceCodeProNerdFont-Regular.ttf", 16), SDL_GetError);
    wheel::Log::assert_(sdl::SDL::init_gamepad(), SDL_GetError);
    sdl::SDL::set_window_position(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    sdl::SDL::set_render_vsync(1);

    context.texture = sdl::SDL::create_texture(config.window_width, config.window_height);
    context.exe_dir = std::filesystem::current_path();
}

void Game::run() {
    const auto dst = SDL_FRect{0, 0, static_cast<float>(config.window_width), static_cast<float>(config.window_height)};
    while (context.running) {
        sdl::SDL::set_render_target(context.texture);
        sdl::SDL::set_color(sdl::SDL::BLACK);
        sdl::SDL::render_clear();

        ecs.update();

        sdl::SDL::set_render_target(nullptr);
        sdl::SDL::set_color(sdl::SDL::BLACK);
        sdl::SDL::render_clear();

        sdl::SDL::render_texture(context.texture, nullptr, &dst);

        sdl::SDL::render_present();
    }

    ecs.clear();
    sdl::SDL::destroy();
}

}  // namespace core
