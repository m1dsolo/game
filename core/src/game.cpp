#include <core/game.hpp>
#include <core/global.hpp>
#include <core/manager/ui.hpp>
#include <core/system/sdl_event.hpp>
#include <core/system/update.hpp>
#include <core/system/transform.hpp>
#include <core/system/animation.hpp>
#include <core/system/render.hpp>
#include <core/system/time.hpp>
#include <core/system/remove_entity.hpp>
#include <core/layer/global.hpp>
#include <core/layer/main_menu.hpp>

#include <wheel/log.hpp>
#include <sdl/sdl.hpp>

#include <rfl/json.hpp>

namespace core {

Game::Game() {
    // SDL
    wheel::Log::assert_(sdl::SDL::init(
        SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_EVENTS | SDL_INIT_GAMEPAD,
        "game", context.virtual_window_width, context.virtual_window_height,
        SDL_WINDOW_BORDERLESS | SDL_WINDOW_RESIZABLE
    ), SDL_GetError);
    wheel::Log::assert_(sdl::SDL::init_audio(), SDL_GetError);
    wheel::Log::assert_(sdl::SDL::init_ttf("assets/font/SauceCodeProNerdFont-Regular.ttf", 16), SDL_GetError);
    wheel::Log::assert_(sdl::SDL::init_gamepad(), SDL_GetError);

    // sdl::SDL::set_window_position(SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    sdl::SDL::set_window_maximized();
    sdl::SDL::set_render_vsync(1);

    std::ifstream file("assets/config/game.json");
    if (file.is_open()) {
        config = rfl::json::read<ConfigResource>(file).value();
    } else {
        wheel::Log::error("Can't find assets/config/game.json!");
    }

    context.texture = sdl::SDL::create_texture(context.virtual_window_width, context.virtual_window_height);
}

void Game::run() {
    ecs.add_systems<
        SDLEventSystem,
        UpdateSystem,
        TransformSystem,
        AnimationSystem,
        RenderSystem,
        TimeSystem,
        RemoveEntitySystem
    >();

    UIManager::instance().push_back<
        GlobalLayer,
        MainMenuLayer
    >();

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
