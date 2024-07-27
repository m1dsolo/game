#pragma once

#include <game/global.hpp>

namespace wheel {

class Action {
public:
    virtual void start() = 0;
    virtual void finish() = 0;

    const std::string& name() const { return name_; }
    SDL_Keycode keycode() const { return keycode_; }

protected:
    Action(const std::string& name, SDL_Keycode keycode) : name_(name), keycode_(keycode) {}

    std::string name_;
    SDL_Keycode keycode_ = 0;
};

class OneShotAction : public Action {
public:
    OneShotAction(const std::string& name, SDL_Keycode keycode, std::function<int()> get_cooldown = nullptr, std::function<void()> start_func = nullptr, std::function<void()> finish_func = nullptr)
        : Action(name, keycode), get_cooldown_(get_cooldown), start_func_(start_func), finish_func_(finish_func) {}

    void start() override;
    void finish() override;

protected:
    std::function<int()> get_cooldown_;
    std::function<void()> start_func_;
    std::function<void()> finish_func_;

private:
    bool ready_ = true;
    bool start_ = false;
};

class MultiShotAction : public Action {
public:
    MultiShotAction(const std::string& name, SDL_Keycode keycode, std::function<int()> get_cooldown = nullptr, std::function<void()> func = nullptr)
        : Action(name, keycode), get_cooldown_(get_cooldown), func_(func) {}

    void start() override;
    void finish() override;

protected:
    std::function<int()> get_cooldown_;
    std::function<void()> func_;
    std::function<void()> timer_func_;

private:
    bool ready_ = true;
    bool start_ = false;
};

class QuitAction : public OneShotAction {
public:
    QuitAction();
};

class MoveUpAction : public OneShotAction {
public:
    MoveUpAction(Entity entity);
};

class MoveDownAction : public OneShotAction {
public:
    MoveDownAction(Entity entity);
};

class MoveLeftAction : public OneShotAction {
public:
    MoveLeftAction(Entity entity);
};

class MoveRightAction : public OneShotAction {
public:
    MoveRightAction(Entity entity);
};

class FlashAction : public OneShotAction {
public:
    FlashAction(Entity entity);
};

class SwitchSelectedItemAction : public OneShotAction {
public:
    SwitchSelectedItemAction(Entity entity, int idx);
};

}  // namespace wheel
