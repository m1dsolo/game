#pragma once

#include <wheel/ecs.hpp>
#include <wheel/geometry.hpp>

namespace wheel {

struct SpawnEvent {
};

struct MouseButtonDownEvent {
    Vector2D<float> pos;
};

struct LevelUpEvent {
    Entity entity;
};

struct PopLayerEvent {
};

struct SwapStageEvent {
};

struct EnemyDeadEvent {
};

}  // namespace wheel
