#pragma once

#include <string>

namespace core {

class BaseSystem {
public:
    virtual ~BaseSystem() = default;

    virtual void update() {
        if (!running_) {
            return;
        }
        update_impl();
    }

    virtual void update_impl() {}

    const std::string name() const { return name_; }

    void pause() { running_ = false; }
    void resume() { running_ = true; }

protected:
    BaseSystem(const std::string& name) : name_(name) {}

private:
    std::string name_;
    bool running_ = true;
};

}  // namespace core
