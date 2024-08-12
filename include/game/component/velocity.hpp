#pragma once

namespace wheel {

struct VelocityComponent {
    VelocityComponent(float max_speed) : speed(max_speed), max_speed(max_speed) {}
    VelocityComponent(float speed, float max_speed) : speed(speed), max_speed(max_speed) {}

    float max_speed = 0.;  // 1s pixel
    float speed = 0.;
};

}  // namespace wheel
