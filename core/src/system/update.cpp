#include <core/system/update.hpp>
#include <core/manager/ui.hpp>

namespace core {

void UpdateSystem::operator()() {
    UIManager::instance().update();
}

}  // namespace core
