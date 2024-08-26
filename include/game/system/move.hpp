#pragma once

#include <game/system/base.hpp>
#include <game/map.hpp>

#include <game/a_star.hpp>

namespace wheel {

class MoveSystem : public BaseSystem<MoveSystem> {
    friend class BaseSystem<MoveSystem>;

public:
    void execute_impl() override;

private:
    MoveSystem() : BaseSystem(false), a_star(Map::instance().planted_structure(), 8) {}
    ~MoveSystem() = default;
    MoveSystem(const MoveSystem&) = delete;

    void calc_input_direction();
    void calc_direction_by_track_nearest_player();
    void calc_direction_by_a_star_track_nearest_player();
    void move();
    void clamp_player_position();

    AStar a_star;
};

}  // namespace wheel
