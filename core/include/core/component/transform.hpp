#pragma once

#include <wheel/geometry.hpp>
#include <sdl/sdl.hpp>

namespace core {

struct Transform {
    wheel::Vector2D<float> position{0.f, 0.f};
    wheel::Vector2D<float> size{0.f, 0.f};
    wheel::Vector2D<float> scale{1.f, 1.f};
    float angle = 0.f;

    SDL_FRect rect() const {
        return {
            position[0] - size[0] / 2,
            position[1] - size[1] / 2,
            size[0],
            size[1],
        };
    }
};

struct TransformComponent {
    TransformComponent(
        wheel::Vector2D<float> position = {0.f, 0.f},
        wheel::Vector2D<float> size = {0.f, 0.f},
        wheel::Vector2D<float> scale = {1.f, 1.f}
    ) : local{position, size, scale} {}

    Transform local;
    Transform global;
};

}  // namespace core
