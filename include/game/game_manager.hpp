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

    int& enemy_cnt() { return enemy_cnt_; }

private:
    GameManager();
    ~GameManager();

    bool running_ = true;
    bool paused_ = false;

    bool stage_ = 0;  // 0 is construction, 1 is combat

    // if enemy_cnt_ == 0 and combat_time_over then swap stage
    int enemy_cnt_ = 0;
    bool combat_time_over_ = false;
};

}  // namespace game
