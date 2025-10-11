#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct MoveSystem {
    void operator()(wheel::ECS& ecs);

    inline static bool is_move_up{false};
    inline static bool is_move_down{false};
    inline static bool is_move_left{false};
    inline static bool is_move_right{false};
};

}  // namespace core
