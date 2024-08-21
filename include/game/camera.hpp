#pragma once

#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>

namespace wheel {

class Camera : public Singleton<Camera> {
    friend class Singleton<Camera>;

public:
    bool update_pos();
    void shake(float strenth, int frequence, int duration);  // duration frames

    Vector2D<float> world2screen(const Vector2D<float>& position) { return position - pos_; }
    Vector2D<float> screen2world(const Vector2D<float>& position) { return position + pos_; }

    Vector2D<float>& pos() { return pos_; }
    Vector2D<float>& size() { return size_; }
    Rect<float> rect() { return {pos_, size_}; }

private:
    Camera();
    ~Camera() = default;
    Camera(const Camera&) = delete;

    Vector2D<float> stable_pos_ {0., 0.};
    Vector2D<float> shake_pos_ {0., 0.};
    Vector2D<float> pos_ {0., 0.};
    Vector2D<float> size_;
    bool shaking_ = false;
};

}  // namespace wheel
