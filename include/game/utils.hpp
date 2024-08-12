#pragma once

#include <vector>
#include <string_view>

#include <SDL3/SDL.h>

#include <wheel/geometry.hpp>

#include <game/action.hpp>
#include <game/component/input.hpp>
#include <game/component/actions.hpp>

namespace wheel {

class GameUtils {
public:
    static int get_file_count(std::string_view path);
    static std::vector<SDL_Texture*> read_textures(std::string_view path);
    static Vector2D<float> gen_spawn_boundary_position();
    static Vector2D<float> gen_spawn_internal_position(int margin = 0);
    static int direction2orientation(const Vector2D<float>& direction);

    template <typename ActionType, typename... Args> requires std::derived_from<ActionType, Action>
    static void emplace_action(Entity entity, Args&&... args) {
        std::shared_ptr<ActionType> action;
        if constexpr (std::is_constructible_v<ActionType, Entity, Args...>) {
            action = std::make_unique<ActionType>(entity, std::forward<Args>(args)...);
        } else {
            action = std::make_unique<ActionType>(std::forward<Args>(args)...);
        }
        if (ecs.has_components<InputComponent>()) {
            auto& input = ecs.get_component<InputComponent>(entity);
            input.key_bindings[action->keycode()] = action->name();
        }
        auto& actions = ecs.get_component<ActionsComponent>(entity);
        actions.action_map[action->name()] = std::move(action);
    }
};

}  // namespace wheel
