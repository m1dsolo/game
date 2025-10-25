#include <core/component/sprite.hpp>
#include <core/manager/sprite.hpp>

namespace core {

SpriteComponent::SpriteComponent(wheel::ID id)
    : sprite(&SpriteManager::instance().get(id)) {}

SpriteComponent::SpriteComponent(SDL_FColor color)
    : sprite(&SpriteManager::instance().get(color)) {}

}  // namespace core
