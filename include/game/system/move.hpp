#pragma once

#include <game/system/base.hpp>

namespace wheel {

class MoveSystem : public BaseSystem<MoveSystem> {
    friend class BaseSystem<MoveSystem>;

public:
    void execute_impl() override;

private:
    MoveSystem() : BaseSystem(false) {}
    ~MoveSystem() = default;
    MoveSystem(const MoveSystem&) = delete;

    void calc_input_direction();
    void calc_direction_by_track_nearest_enemy();
    void move();
    void clamp_player_position();
};

}  // namespace wheel
