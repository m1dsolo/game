#include <core/system/render.hpp>
#include <core/manager/ui.hpp>

namespace core {

void RenderSystem::update_impl() {
    UIManager::instance().render();
}

}  // namespace core
