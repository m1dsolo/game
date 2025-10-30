#include <core/game.hpp>
#include <core/global.hpp>
#include <core/manager/layer.hpp>
#include <core/manager/sprite.hpp>
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
#include <core/resource/config.hpp>
#include <core/resource/context.hpp>
#include <core/resource/trigger.hpp>
#include <core/resource/time.hpp>
#include <core/resource/input.hpp>
#include <core/resource/inventory.hpp>

#include <wheel/log.hpp>
#include <sdl/sdl.hpp>

#include <rfl/json.hpp>

namespace core {

Game::Game() {
    ContextResource context;

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
        auto config = rfl::json::read<ConfigResource>(file).value();
        ecs.add_resource(std::move(config));
    } else {
        wheel::Log::error("Can't find assets/config/game.json!");
    }

    context.texture = sdl::SDL::create_texture(context.virtual_window_width, context.virtual_window_height);
    ecs.add_resource<ContextResource>(std::move(context));
    ecs.add_resource(TriggerResource{});
    ecs.add_resource(TimeResource{});
    ecs.add_resource(InputResource{});
    ecs.add_resource(InventoryResource{});
}

void Game::run() {
    SpriteManager::instance();

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

    LayerManager::instance().push("GlobalLayer");
    LayerManager::instance().push("MainMenuLayer");

    auto& context = ecs.get_resource<ContextResource>();

    while (context.running) {
        sdl::SDL::set_render_target(context.texture);
        sdl::SDL::set_render_color(sdl::SDL::Color::Black);
        sdl::SDL::render_clear();

        ecs.update();

        sdl::SDL::set_render_target(nullptr);
        sdl::SDL::set_render_color(sdl::SDL::Color::Black);
        sdl::SDL::render_clear();

        sdl::SDL::render_texture(context.texture, nullptr, &context.viewport);

        sdl::SDL::render_present();
    }

    ecs.clear();
    sdl::SDL::destroy();
}

}  // namespace core
