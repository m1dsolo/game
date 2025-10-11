#pragma once

namespace wheel {
class ECS;
}

namespace core {

struct RenderSystem {
    void operator()(wheel::ECS& ecs);

private:

    // 0. map
    // 1. player, enemy, range, aura
    // 2. hp_bar, bullet
    // 3. button
    // 4. text
    static const unsigned int MAX_LAYER = 4;
};

}  // namespace core
