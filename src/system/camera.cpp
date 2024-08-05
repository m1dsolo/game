#include <game/system/camera.hpp>

#include <game/global.hpp>

namespace wheel {

void CameraSystem::execute_impl() {
    camera.update_pos();
}

}  // namespace wheel
