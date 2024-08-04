#include <game/camera.hpp>

#include <game/global.hpp>

#include <game/component/self.hpp>
#include <game/component/position.hpp>

namespace wheel {

Camera::Camera() : size_(config_resource.w, config_resource.h) {}

void Camera::update_pos() {
    Entity entity = ecs.get_entities<SelfComponent>()[0];
    auto [x, y] = ecs.get_component<PositionComponent>(entity).vec;
    pos_ = Vector2D<double>(x - size_.x / 2., y - size_.y / 2.);
}

void Camera::shake(double strenth, int frequence, int duration) {
    timer_resource.add(frequence, duration, [this, strenth]() {
        pos_ = Vector2D<double>(random.uniform(-strenth, strenth), random.uniform(-strenth, strenth));
    });
    auto cur_pos = pos_;
    timer_resource.add(frequence * duration, 1, [this, cur_pos]() {
        pos_ = cur_pos;
    });
}

}  // namespace wheel
