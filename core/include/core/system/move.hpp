#pragma once

#include <core/system/base.hpp>

namespace core {

class MoveSystem : public BaseSystem {
public:
    MoveSystem() : BaseSystem("Move") {}

    void update_impl() override;

private:
    void process_event_();
    void update_move_direction_();
    void move_();

    bool is_move_up_{false};
    bool is_move_down_{false};
    bool is_move_left_{false};
    bool is_move_right_{false};
};

}  // namespace core
