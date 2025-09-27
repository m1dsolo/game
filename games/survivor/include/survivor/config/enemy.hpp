#pragma once

#include <string>
#include <vector>

namespace survivor {

struct RewardConfig {
    std::string name;
    std::pair<int, int> count;
    float chance = 1.0f;
};

struct EnemyConfig {
    std::string name;
    int hp = 100;
    float speed = 100.f;
    std::vector<RewardConfig> rewards;
};

}  // namespace survivor
