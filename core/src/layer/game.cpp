#include <core/layer/game.hpp>
#include <core/global.hpp>
#include <core/util/coordinate.hpp>
#include <core/component/transform.hpp>
#include <core/component/direction.hpp>
#include <core/component/sprite.hpp>
#include <core/component/render.hpp>
#include <core/tag/input.hpp>

#include <sdl/sdl.hpp>

namespace core {

void GameLayer::on_attach() {
    player_entity_ = ecs.get_entity<InputTag>();
}

void GameLayer::on_render() {
    for (int layer = 0; layer <= MAX_LAYER; layer++) {
        for (auto [render, transform, sprite]
            : ecs.get_components<RenderComponent, TransformComponent, SpriteComponent>()) {
            if (render.layer != layer || !render.visible) {
                continue;
            }

            // TODO: not render out of screen?

            // SDL_SetTextureColorModFloat(sprite.texture, sprite.color[0], sprite.color[1], sprite.color[2]);
            // SDL_SetTextureAlphaModFloat(sprite.texture, sprite.color[3]);
            auto dst = Coordinate::world2screen(transform.global.rect());
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

bool GameLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_W: is_move_up_ = true; break;
                case SDLK_S: is_move_down_ = true; break;
                case SDLK_A: is_move_left_ = true; break;
                case SDLK_D: is_move_right_ = true; break;
            }
            break;
        }
        case SDL_EVENT_KEY_UP: {
            switch (event.key.key) {
                case SDLK_W: is_move_up_ = false; break;
                case SDLK_S: is_move_down_ = false; break;
                case SDLK_A: is_move_left_ = false; break;
                case SDLK_D: is_move_right_ = false; break;
            }
            break;
        }
        // case SDL_Event_GAMEPAD_ADDED
        case SDL_EVENT_GAMEPAD_AXIS_MOTION: {
            const auto& gaxis = event.gaxis;
            switch (gaxis.axis) {
                case SDL_GAMEPAD_AXIS_LEFTX: {
                    float value = gaxis.value / 32767.0f;
                    is_move_left_ = (value < -0.25);
                    is_move_right_ = (value > 0.25);
                    break;
                }
                case SDL_GAMEPAD_AXIS_LEFTY: {
                    float value = gaxis.value / 32767.0f;
                    is_move_up_ = (value < -0.25);
                    is_move_down_ = (value > 0.25);
                    break;
                }
            }
        }
    }

    auto& direction = ecs.get_component<DirectionComponent>(player_entity_);
    auto d = wheel::Vector2D<float>{
        static_cast<float>(is_move_right_) - is_move_left_,
        static_cast<float>(is_move_down_) - is_move_up_,
    }.normalize();
    direction.move = d;
    if (d != 0.f) {
        direction.look = d;
    }

    return d != 0;
}

}  // namespace core
