#include <core/system/render.hpp>
#include <core/global.hpp>
#include <core/manager/ui.hpp>
#include <core/component/transform.hpp>
#include <core/component/sprite.hpp>
#include <core/component/layer.hpp>
#include <core/tag/input.hpp>
#include <core/tag/render.hpp>
#include <core/util/coordinate.hpp>

namespace core {

void RenderSystem::operator()() {
    for (int layer = 0; layer <= MAX_LAYER; layer++) {
        for (auto [render_layer, transform, sprite, _]
            : ecs.get_components<LayerComponent, TransformComponent, SpriteComponent, RenderTag>()) {
            if (render_layer.layer != layer) {
                continue;
            }

            // TODO: not render out of screen?

            if (sprite.get_hit_effect_cnt > 0) {
                sdl::SDL::set_blend_mode(sprite.sprite->texture, SDL_BLENDMODE_ADD);
            }
            sdl::SDL::TextureColorGuard color_guard(sprite.sprite->texture, sprite.color);
            auto dst = transform.global.rect();
            if (transform.type == Coordinate::Type::World) {
                dst = Coordinate::world2screen(dst);
            }
            sdl::SDL::render_texture(sprite.sprite->texture, &sprite.sprite->rect, &dst, sprite.flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
            sdl::SDL::set_blend_mode(sprite.sprite->texture, SDL_BLENDMODE_BLEND);
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
