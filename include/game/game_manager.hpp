#pragma once

#include <wheel/singleton.hpp>

namespace wheel {

class GameManager : public Singleton<GameManager> {
    friend class Singleton<GameManager>;

public:
    void run();
    void quit();
    void pause();
    void resume();
    void swap_stage();  // swap between combat and construction stage

    bool paused() const { return paused_; }

private:
    GameManager();
    ~GameManager();

    bool running_ = true;
    bool paused_ = false;
};

}  // namespace game
