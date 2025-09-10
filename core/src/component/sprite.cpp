#include <core/component/sprite.hpp>
#include <core/manager/sprite.hpp>

namespace core {

SpriteComponent::SpriteComponent(const std::string& name)
    : sprite(&SpriteManager::instance().get(name)) {}

}  // namespace core
