#include <core/system/collider.hpp>
#include <core/manager/collider.hpp>

namespace core {

void ColliderSystem::operator()() {
    ColliderManager::instance().update();
}

}  // namespace core
