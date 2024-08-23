#include <game/layer/game.hpp>

#include <game/global.hpp>
#include <game/ui.hpp>
#include <game/event.hpp>
#include <game/game_manager.hpp>
#include <game/scene_manager.hpp>
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
#include <game/component/actions.hpp>
#include <game/component/input.hpp>
#include <game/component/position.hpp>
#include <game/component/direction.hpp>

namespace wheel {

void GameLayer::on_attach() {
    self_entity_ = ecs.get_entities<SelfComponent>()[0];
}

void GameLayer::on_detach() {
}

void GameLayer::on_update() {
}

void GameLayer::on_render() {
    auto& scene_manager = SceneManager::instance();
    for (Entity entity : scene_manager.query()) {
        auto& texture = ecs.get_component<TextureComponent>(entity).texture;
        const auto& pos = ecs.get_component<PositionComponent>(entity).vec;
        const auto& size = ecs.get_component<SizeComponent>(entity).vec;
        auto left_top = pos - size / 2 - camera.left_top();
        SDL_FRect dst = {left_top.x, left_top.y, size.x, size.y};
        sdl.render(texture, nullptr, &dst);
    }
}

bool GameLayer::on_event(const SDL_Event& event) {
    // interrupt continuous actions
    if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        Entity entity = ecs.get_entities<SelfComponent>()[0];
        if (ecs.has_components<ContinuousActionComponent>(entity)) {
            ecs.del_components<ContinuousActionComponent>(entity);
        }
    }

    auto& action_map = ecs.get_component<ActionsComponent>(self_entity_).action_map;
    auto& key_bindings = ecs.get_component<InputComponent>(self_entity_).key_bindings;
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

            if (key_bindings.count(keycode)) {
                const std::string& action_name = key_bindings.at(keycode);
                action_map.at(action_name)->start();
            }
            return true;
        }
        case SDL_EVENT_KEY_UP: {
            auto keycode = event.key.key;
            if (key_bindings.count(keycode)) {
                const std::string& action_name = key_bindings.at(keycode);
                action_map.at(action_name)->finish();
            }
            return true;
        }
        case SDL_EVENT_MOUSE_MOTION: {
            float x = event.motion.x, y = event.motion.y;
            auto& camera_size = camera.size();  // because player in middle of camera
            Entity entity = ecs.get_entities<SelfComponent>()[0];
            auto& aim_direction = ecs.get_component<AimDirectionComponent>(entity);
            aim_direction.vec = (Vector2D<float>{x, y} - camera_size / 2).normalize();
            return false;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            float x = event.motion.x, y = event.motion.y;
            switch (event.button.button) {
                case SDL_BUTTON_LEFT: {
                    if (key_bindings.count(SDLK_MOUSE_LEFT)) {
                        const std::string& action_name = key_bindings.at(SDLK_MOUSE_LEFT);
                        action_map.at(action_name)->start();
                        action_map.at(action_name)->start(x, y);
                    }
                    break;
                }
                case SDL_BUTTON_MIDDLE: {
                    if (key_bindings.count(SDLK_MOUSE_MIDDLE)) {
                        const std::string& action_name = key_bindings.at(SDLK_MOUSE_MIDDLE);
                        action_map.at(action_name)->start();
                        action_map.at(action_name)->start(x, y);
                    }
                    break;
                }
                case SDL_BUTTON_RIGHT: {
                    if (key_bindings.count(SDLK_MOUSE_RIGHT)) {
                        const std::string& action_name = key_bindings.at(SDLK_MOUSE_RIGHT);
                        action_map.at(action_name)->start();
                        action_map.at(action_name)->start(x, y);
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
                    if (key_bindings.count(SDLK_MOUSE_LEFT)) {
                        const std::string& action_name = key_bindings.at(SDLK_MOUSE_LEFT);
                        action_map.at(action_name)->finish();
                    }
                    break;
                }
                case SDL_BUTTON_MIDDLE: {
                    if (key_bindings.count(SDLK_MOUSE_MIDDLE)) {
                        const std::string& action_name = key_bindings.at(SDLK_MOUSE_MIDDLE);
                        action_map.at(action_name)->finish();
                    }
                    break;
                }
                case SDL_BUTTON_RIGHT: {
                    if (key_bindings.count(SDLK_MOUSE_RIGHT)) {
                        const std::string& action_name = key_bindings.at(SDLK_MOUSE_RIGHT);
                        action_map.at(action_name)->finish();
                    }
                    break;
                }
            }
            return true;
        }
        case SDL_EVENT_WINDOW_RESIZED: {
            camera.size() = {(float)event.window.data1, (float)event.window.data2};
            return true;
        }
    }

    return false;
}

}  // namespace wheel
