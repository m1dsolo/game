#pragma once

#include <vector>

namespace wheel {

struct ConfigResource {
    ConfigResource();
    ~ConfigResource();

    int fps = 60;
    std::vector<int> levelup_exps;

    int w = 1920;
    int h = 1080;
};

}  // namespace wheel
