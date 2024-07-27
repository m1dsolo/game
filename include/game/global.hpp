#pragma once

#include <wheel/ecs.hpp>

#include <game/sdl.hpp>

#include <game/resource/config.hpp>
#include <game/resource/game.hpp>
#include <game/resource/player.hpp>
#include <game/resource/map.hpp>
#include <game/resource/timer.hpp>

namespace wheel {

inline auto& ecs = ECS::instance();

inline auto& sdl = SDL::instance();
inline auto& renderer = sdl.renderer;
inline auto& font = sdl.font;

inline auto ecs_init_config_resource = ([]() {
    ecs.add_resource<ConfigResource>();
}(), 0);
inline auto& config_resource = ecs.get_resource<ConfigResource>();

inline auto ecs_init_game_resource = ([]() {
    ecs.add_resource<GameResource>();
}(), 0);
inline auto& game_resource = ecs.get_resource<GameResource>();
inline auto& random = game_resource.random;

inline auto ecs_init_player_resource = ([]() {
    ecs.add_resource<PlayerResource>();
}(), 0);
inline auto& player_resource = ecs.get_resource<PlayerResource>();

inline auto ecs_init_map_resource = ([]() {
    ecs.add_resource<MapResource>();
}(), 0);
inline auto& map_resource = ecs.get_resource<MapResource>();

inline auto ecs_init_timer_resource = ([]() {
    ecs.add_resource<TimerResource>();
}(), 0);
inline auto& timer_resource = ecs.get_resource<TimerResource>();
inline auto& timer = timer_resource.timer;

}  // namespace wheel
