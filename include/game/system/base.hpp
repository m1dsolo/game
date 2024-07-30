#pragma once

#include <wheel/singleton.hpp>

#include <game/game_manager.hpp>

namespace wheel {

template <typename T>
class BaseSystem {
public:
    static T& instance() {
        static T instance;
        return instance;
    }

    void execute() {
        if (!always_run_ && GameManager::instance().paused()) {
            return;
        }
        execute_impl();
    }

    virtual void execute_impl() {}

protected:
    BaseSystem(bool always_run) : always_run_(always_run) {}
    virtual ~BaseSystem() = default;

private:
    bool always_run_;
};

}  // namespace wheel
