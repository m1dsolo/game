#pragma once

#include <game/system/base.hpp>

namespace wheel {

class CameraSystem : public BaseSystem<CameraSystem> {
    friend class BaseSystem<CameraSystem>;

public:
    void execute_impl() override;

private:
    CameraSystem() : BaseSystem(true) {}
    ~CameraSystem() = default;
    CameraSystem(const CameraSystem&) = delete;
};

}  // namespace wheel

