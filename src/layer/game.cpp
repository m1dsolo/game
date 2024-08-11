#include <game/layer/game.hpp>

#include <game/global.hpp>
#include <game/ui.hpp>
#include <game/event.hpp>
#include <game/game_manager.hpp>
#include <game/sdl.hpp>

#include <game/layer/exit.hpp>

#include <game/component/self.hpp>
#include <game/component/position.hpp>
#include <game/component/size.hpp>
#include <game/component/texture.hpp>
#include <game/component/health_bar.hpp>
#include <game/component/hp.hpp>
#include <game/component/tag.hpp>
#include <game/component/aim_direction.hpp>
#include <game/component/continuous_action.hpp>
#include <game/component/floor.hpp>

namespace wheel {

void GameLayer::on_attach() {
    Entity entity = ecs.get_entities<SelfComponent>()[0];
    action_map_ = &ecs.get_component<ActionsComponent>(entity).action_map;
    key_bindings = &ecs.get_component<InputComponent>(entity).key_bindings;
    position_ = &ecs.get_component<PositionComponent>(entity).vec;
    aim_direction_ = &ecs.get_component<AimDirectionComponent>(entity).vec;
}

void GameLayer::on_detach() {
}

void GameLayer::on_update() {
}

void GameLayer::on_render() {
    render_texture();
    render_health_bar();
}

bool GameLayer::on_event(const SDL_Event& event) {
    // interrupt continuous actions
    if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        Entity entity = ecs.get_entities<SelfComponent>()[0];
        if (ecs.has_components<ContinuousActionComponent>(entity)) {
            ecs.del_components<ContinuousActionComponent>(entity);
        }
    }

    switch (event.type) {
        case SDL_EVENT_QUIT: {
            GameManager::instance().quit();
            return true;
        }
        case SDL_EVENT_KEY_DOWN: {
            auto keycode = event.key.key;
            if (keycode == SDLK_ESCAPE) {
                UI::instance().push_back<ExitLayer>();
                break;
            }

            if (key_bindings->count(keycode)) {
                const std::string& action_name = key_bindings->at(keycode);
                action_map_->at(action_name)->start();
            }
            return true;
        }
        case SDL_EVENT_KEY_UP: {
            auto keycode = event.key.key;
            if (key_bindings->count(keycode)) {
                const std::string& action_name = key_bindings->at(keycode);
                action_map_->at(action_name)->finish();
            }
            return true;
        }
        case SDL_EVENT_MOUSE_MOTION: {
            float x = event.motion.x, y = event.motion.y;
            auto [w, h] = camera.size();
            *aim_direction_ = (Vector2D<double>{x, y} - Vector2D<double>{w / 2., h / 2.}).normalize();
            return false;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            float x = event.motion.x, y = event.motion.y;
            switch (event.button.button) {
                case SDL_BUTTON_LEFT: {
                    if (key_bindings->count(SDLK_MOUSE_LEFT)) {
                        const std::string& action_name = key_bindings->at(SDLK_MOUSE_LEFT);
                        action_map_->at(action_name)->start();
                        action_map_->at(action_name)->start(x, y);
                    }
                    break;
                }
                case SDL_BUTTON_MIDDLE: {
                    if (key_bindings->count(SDLK_MOUSE_MIDDLE)) {
                        const std::string& action_name = key_bindings->at(SDLK_MOUSE_MIDDLE);
                        action_map_->at(action_name)->start();
                        action_map_->at(action_name)->start(x, y);
                    }
                    break;
                }
                case SDL_BUTTON_RIGHT: {
                    if (key_bindings->count(SDLK_MOUSE_RIGHT)) {
                        const std::string& action_name = key_bindings->at(SDLK_MOUSE_RIGHT);
                        action_map_->at(action_name)->start();
                        action_map_->at(action_name)->start(x, y);
                    }
                    break;
                }
            }
            return true;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            float x = event.motion.x, y = event.motion.y;
            switch (event.button.button) {
                case SDL_BUTTON_LEFT: {
                    if (key_bindings->count(SDLK_MOUSE_LEFT)) {
                        const std::string& action_name = key_bindings->at(SDLK_MOUSE_LEFT);
                        action_map_->at(action_name)->finish();
                    }
                    break;
                }
                case SDL_BUTTON_MIDDLE: {
                    if (key_bindings->count(SDLK_MOUSE_MIDDLE)) {
                        const std::string& action_name = key_bindings->at(SDLK_MOUSE_MIDDLE);
                        action_map_->at(action_name)->finish();
                    }
                    break;
                }
                case SDL_BUTTON_RIGHT: {
                    if (key_bindings->count(SDLK_MOUSE_RIGHT)) {
                        const std::string& action_name = key_bindings->at(SDLK_MOUSE_RIGHT);
                        action_map_->at(action_name)->finish();
                    }
                    break;
                }
            }
            return true;
        }
        case SDL_EVENT_WINDOW_RESIZED: {
            camera.size() = {event.window.data1, event.window.data2};
            return true;
        }
    }

    return false;
}

void GameLayer::render_texture() {
    // animation(TODO: bad performance now)
    for (auto [_, position, size, texture] : ecs.get_components<FloorComponent, PositionComponent, SizeComponent, TextureComponent>()) {
        if (!texture.texture) {
            continue;
        }
        auto left_top = position.vec - Vector2D<double>{size.w / 2., size.h / 2.} - camera.pos();
        SDL_FRect dst_frect = {(float)left_top.x, (float)left_top.y, (float)size.w, (float)size.h};
        sdl.render(texture.texture, nullptr, &dst_frect);
    }

    for (auto [entity, position, size, texture] : ecs.get_entity_and_components<PositionComponent, SizeComponent, TextureComponent>()) {
        if (ecs.has_components<FloorComponent>(entity) || !texture.texture) {
            continue;
        }
        auto left_top = position.vec - Vector2D<double>{size.w / 2., size.h / 2.} - camera.pos();
        SDL_FRect dst_frect = {(float)left_top.x, (float)left_top.y, (float)size.w, (float)size.h};
        sdl.render(texture.texture, nullptr, &dst_frect);
    }
}

// TODO: merge to render_texture
void GameLayer::render_health_bar() {
    for (auto [health_bar, position, size] : ecs.get_components<HealthBarComponent, PositionComponent, SizeComponent>()) {
        auto& hp = ecs.get_component<HPComponent>(health_bar.master);

        auto left_top = position.vec - camera.pos();
        SDL_FRect dst = {(float)left_top.x, (float)left_top.y, (float)size.w, (float)size.h};
        sdl.draw_rect(&dst, sdl.RED);
        dst.w = (float)size.w * hp.hp / hp.max_hp;
        sdl.draw_rect(&dst, sdl.GREEN);
    }
}

}  // namespace wheel
