#include <game/camera.hpp>

#include <game/global.hpp>

#include <wheel/utils.hpp>

#include <game/component/self.hpp>
#include <game/component/position.hpp>

namespace wheel {

Camera::Camera() : size_(config_resource.w, config_resource.h), rect_(0, 0, config_resource.w, config_resource.h) {}

bool Camera::update_pos() {
    Entity entity = ecs.get_entities<SelfComponent>()[0];
    stable_pos_ = ecs.get_component<PositionComponent>(entity).vec;
    if (auto new_pos = stable_pos_ + shake_pos_; new_pos != pos_) {
        pos_ = new_pos;
        return true;
    } else {
        return false;
    }
}

void Camera::shake(float strenth, int frequence, int duration) {
    if (shaking_) {
        return;
    }

    shaking_ = true;
    timer_resource.add(frequence, duration, [this, strenth]() {
        shake_pos_ = Vector2D<float>(random.uniform<float>(-strenth, strenth), random.uniform<float>(-strenth, strenth));
    }, true);
    timer_resource.add(frequence * duration, 1, [this, strenth]() {
        shaking_ = false;
        shake_pos_ = {0., 0.};
    });
}

}  // namespace wheel
