#pragma once

namespace wheel {

struct HPComponent {
    HPComponent(int max_hp) : hp(max_hp), max_hp(max_hp) {}
    HPComponent(int hp, int max_hp) : hp(hp), max_hp(max_hp) {}

    int hp;
    int max_hp;
};

}  // namespace wheel

