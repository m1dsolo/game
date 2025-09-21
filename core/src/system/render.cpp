#include <core/system/render.hpp>
#include <core/global.hpp>
#include <core/manager/ui.hpp>
#include <core/util/coordinate.hpp>
#include <core/component/transform.hpp>
#include <core/component/sprite.hpp>
#include <core/component/render.hpp>

namespace core {

void RenderSystem::operator()() {
    for (int layer = 0; layer <= MAX_LAYER; layer++) {
        for (auto [render, transform, sprite]
            : ecs.get_components<RenderComponent, TransformComponent, SpriteComponent>()) {
            if (render.layer != layer || !render.visible) {
                continue;
            }

            // TODO: not render out of screen?

            // SDL_SetTextureColorModFloat(sprite.texture, sprite.color[0], sprite.color[1], sprite.color[2]);
            // SDL_SetTextureAlphaModFloat(sprite.texture, sprite.color[3]);
            auto dst = transform.global.rect();
            if (transform.type == Coordinate::Type::WORLD) {
                dst = Coordinate::world2screen(dst);
            }
            sdl::SDL::render_texture(sprite.sprite->texture, &sprite.sprite->rect, &dst);
            // SDL_RenderTextureRotated(
            //     sdl::SDL::renderer(),
            //     sprite.texture,
            //     &src,
            //     &dst,
            //     transform.angle,
            //     &transform.center,
            //     SDL_FLIP_NONE
            // );
        }
    }
}

}  // namespace core
