#include <game/system/render.hpp>

namespace wheel {

void RenderSystem::execute_impl() {
    ui_.update();
    ui_.render();
}

}  // namespace wheel
