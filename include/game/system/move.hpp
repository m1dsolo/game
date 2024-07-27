#pragma once

#include <wheel/singleton.hpp>

namespace wheel {

class MoveSystem : public Singleton<MoveSystem> {
    friend class Singleton<MoveSystem>;

public:
    void execute();

private:
    MoveSystem() = default;
    ~MoveSystem() = default;
    MoveSystem(const MoveSystem&) = delete;

    void calc_input_direction();
    void calc_direction_by_track_nearest_enemy();
    void move();
    void clamp_player_position();
};

}  // namespace wheel
