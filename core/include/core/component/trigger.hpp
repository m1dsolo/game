#pragma once

#include <core/component/collider.hpp>

#include <ecs/entity.hpp>

#include <functional>
#include <unordered_set>

namespace core {

struct TriggerComponent : ColliderComponent {
    TriggerComponent(
        wheel::Vector2D<float> size = {0.f, 0.f},
        bool dynamic = true,
        std::function<void(wheel::Entity, wheel::Entity)> on_enter = nullptr,
        std::function<void(wheel::Entity, wheel::Entity)> on_stay = nullptr,
        std::function<void(wheel::Entity, wheel::Entity)> on_exit = nullptr
    );

    std::function<void(wheel::Entity entity, wheel::Entity other)> on_enter;
    std::function<void(wheel::Entity entity, wheel::Entity other)> on_stay;
    std::function<void(wheel::Entity entity, wheel::Entity other)> on_exit;

    std::unordered_set<wheel::Entity> stay_entities;
};

}  // namespace core
