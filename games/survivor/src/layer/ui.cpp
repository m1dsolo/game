#include <survivor/layer/ui.hpp>
#include <core/global.hpp>
#include <core/manager/texture.hpp>
#include <core/component/transform.hpp>
#include <core/tag/input.hpp>
#include <survivor/component/hp.hpp>

#include <sdl/sdl.hpp>

#include <format>

using namespace core;

namespace survivor {

void UILayer::on_attach() {
    player_entity_ = ecs.get_entity<InputTag>();
}

void UILayer::on_render() {
    // TODO: optimize
    const float x = 0.1;
    const float y = 0.1;

    if (!ecs.has_component<HPComponent>(player_entity_)) {
        return;
    }

    const auto& hp = ecs.get_component<HPComponent>(player_entity_);
    auto text = std::format("hp: {}/{}", hp.hp, hp.max_hp);

    auto& texture_manager = TextureManager::instance();
    if (!texture_manager.has(text)) {
        texture_manager.set(text, sdl::SDL::create_texture(text, 32, sdl::SDL::ORANGE));
    }
    auto texture = texture_manager.get(text);
    auto [w, h] = sdl::SDL::get_texture_size(texture);

    SDL_FRect dst{
        x * config.virtual_window_width,
        y * config.virtual_window_height,
        w,
        h
    };
    auto guard = sdl::SDL::TargetGuard{context.texture};
    sdl::SDL::render_texture(texture, nullptr, &dst);
}

bool UILayer::on_event(const SDL_Event& event) {
    return false;
}

}  // namespace survivor
