#pragma once

#include <string>
#include <vector>

namespace wheel {

struct RewardItem {
    std::string name;
    std::pair<int, int> count;
    float chance;
};

struct RewardComponent {
    int exp;
    // rewarder entity, killer entity
    std::vector<RewardItem> reward_items;
};

}
