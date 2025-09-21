#pragma once

namespace core {

struct RenderComponent {
    int layer = 0;
    bool visible = true;
    int blend_mode_add_cnt = 0;
};

}  // namespace core
