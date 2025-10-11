#pragma once

namespace wheel {
class ECS;
}

namespace survivor {

struct AchievementSystem {
    void operator()(wheel::ECS& ecs);
};

}  // namespace survivor
