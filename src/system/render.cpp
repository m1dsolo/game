#include <game/system/render.hpp>

#include <game/ui.hpp>

#include <game/component/health_bar.hpp>
#include <game/component/size.hpp>
#include <game/component/master.hpp>
#include <game/component/texture.hpp>
#include <game/component/hp.hpp>

namespace wheel {

void RenderSystem::execute_impl() {
    UI::instance().update();
    update_health_bar_texture();
    UI::instance().render();
}

void RenderSystem::update_health_bar_texture() {
    for (auto [_, texture, size, master] : ecs.get_components<HealthBarComponent, TextureComponent, SizeComponent, MasterComponent>()) {
        const auto& hp = ecs.get_component<HPComponent>(master.entity);

        sdl.set_target(texture.texture);
        SDL_FRect dst = {0, 0, size.vec.x, size.vec.y};
        sdl.draw_rect(&dst, sdl.RED);
        dst.w = (float)size.vec.x * std::min(hp.max_hp, hp.hp) / hp.max_hp;
        sdl.draw_rect(&dst, sdl.GREEN);
        sdl.set_target(nullptr);
    }
}

}  // namespace wheel
