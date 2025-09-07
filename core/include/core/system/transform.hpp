#pragma once

#include <core/system/base.hpp>

#include <ecs/entity.hpp>

namespace core {

class TransformComponent;

class TransformSystem : public BaseSystem {
public:
    TransformSystem() : BaseSystem("Transform") {}

    void update_impl() override;

private:
    void update_transform_();
    void update_transform_(wheel::Entity entity, const TransformComponent& parent_transform);
};

}  // namespace core
