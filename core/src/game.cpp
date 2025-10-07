#include <core/game.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/layer.hpp>
#include <core/layer/global.hpp>
#include <core/layer/main_menu.hpp>
#include <core/system/handle_sdl_event.hpp>
#include <core/system/handle_button_event.hpp>
#include <core/system/layer_update.hpp>
#include <core/system/transform.hpp>
#include <core/system/animation.hpp>
#include <core/system/render.hpp>
#include <core/system/time.hpp>
#include <core/system/remove_entity.hpp>
#include <core/component/name.hpp>
#include <core/component/hp.hpp>
#include <core/component/button.hpp>
#include <core/component/layout.hpp>
#include <core/tag/hp_bar.hpp>
#include <core/tag/rigidbody.hpp>
#include <core/tag/obstacle.hpp>

#include <wheel/log.hpp>
#include <sdl/sdl.hpp>

#include <rfl/json.hpp>

namespace core {

void init_sdl_();
void load_config_();

Game::Game() {
    init_sdl_();
    load_config_();

    context.texture = sdl::SDL::create_texture(context.virtual_window_width, context.virtual_window_height);
}

void Game::run() {
    ecs.add_systems<
        HandleSDLEventSystem,
        HandleButtonEventSystem,
        LayerUpdateSystem,
        TransformSystem,
        AnimationSystem,
        RenderSystem,
        TimeSystem,
        RemoveEntitySystem
    >();

    LayerManager::instance().push<
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

void init_sdl_() {
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
}

void load_config_() {
    std::ifstream file("assets/config/game.json");
    if (file.is_open()) {
        config = rfl::json::read<ConfigResource>(file).value();
    } else {
        wheel::Log::error("Can't find assets/config/game.json!");
    }
}

}  // namespace core
