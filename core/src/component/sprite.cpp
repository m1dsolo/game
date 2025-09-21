#include <core/component/sprite.hpp>
#include <core/manager/sprite.hpp>

namespace core {

SpriteComponent::SpriteComponent(const std::string& name)
    : sprite(&SpriteManager::instance().get(name)) {}

SpriteComponent::SpriteComponent(SDL_Color color)
    : sprite(&SpriteManager::instance().get(color)) {}

}  // namespace core
