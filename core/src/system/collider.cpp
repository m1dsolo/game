#include <core/system/collider.hpp>
#include <core/manager/collider.hpp>

namespace core {

void ColliderSystem::update_impl() {
    ColliderManager::instance().update();
}

}  // namespace core
