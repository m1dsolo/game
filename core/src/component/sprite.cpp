#include <core/component/sprite.hpp>
#include <core/manager/sprite.hpp>

namespace core {

SpriteComponent::SpriteComponent(wheel::ID id)
    : id(id), sprite(&SpriteManager::instance().get(id)) {}

}  // namespace core
