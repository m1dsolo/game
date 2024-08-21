#include <game/camera.hpp>

#include <game/global.hpp>

#include <wheel/utils.hpp>

#include <game/component/self.hpp>
#include <game/component/position.hpp>

namespace wheel {

Camera::Camera() : size_(config_resource.w, config_resource.h) {}

bool Camera::update_pos() {
    Entity entity = ecs.get_entities<SelfComponent>()[0];
    auto [x, y] = ecs.get_component<PositionComponent>(entity).vec;
    stable_pos_ = Vector2D<float>(x - size_.x / 2., y - size_.y / 2.);
    if (auto pos = stable_pos_ + shake_pos_; pos != pos_) {
        pos_ = pos;
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
