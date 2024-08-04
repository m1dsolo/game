#pragma once

#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>

namespace wheel {

class Camera : public Singleton<Camera> {
    friend class Singleton<Camera>;

public:
    void update_pos();
    void shake(double strenth, int frequence, int duration);  // duration frames

    Vector2D<double> world2screen(const Vector2D<double>& position) { return position - pos_; }
    Vector2D<double> screen2world(const Vector2D<double>& position) { return position + pos_; }

    Vector2D<double>& pos() { return pos_; }
    Vector2D<int>& size() { return size_; }

private:
    Camera();
    ~Camera() = default;
    Camera(const Camera&) = delete;

    Vector2D<double> pos_ {0., 0.};
    Vector2D<int> size_;
};

}  // namespace wheel
