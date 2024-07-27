#pragma once

namespace wheel {

struct VelocityComponent {
    VelocityComponent(double max_speed) : speed(max_speed), max_speed(max_speed) {}
    VelocityComponent(double speed, double max_speed) : speed(speed), max_speed(max_speed) {}

    double max_speed = 0.;  // 1s pixel
    double speed = 0.;
};

}  // namespace wheel
