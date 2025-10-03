#pragma once

namespace core {

struct HPComponent {
    HPComponent(int max_hp) : max_hp(max_hp), hp(max_hp) {}
    int max_hp = 0;
    int hp = 0;
};

}  // namespace core
