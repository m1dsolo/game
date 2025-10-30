#include <core/system/handle_button_event.hpp>
#include <core/manager/sprite.hpp>
#include <core/manager/audio.hpp>
#include <core/component/button.hpp>
#include <core/component/sprite.hpp>
#include <core/entity_event/button.hpp>

#include <ecs/ecs.hpp>

namespace core {

void HandleButtonEventSystem::operator()(wheel::ECS& ecs) {
    for (auto [button, sprite, _] : ecs.get_components<ButtonComponent, SpriteComponent, ButtonUnhoveredEvent>()) {
        sprite.sprite = &SpriteManager::instance().get(button.normal_color_id);
    }
    for (auto [button, sprite, _] : ecs.get_components<ButtonComponent, SpriteComponent, ButtonReleasedEvent>()) {
        sprite.sprite = &SpriteManager::instance().get(button.is_hovered ? button.hovered_color_id : button.normal_color_id);
    }
    for (auto [button, sprite, _] : ecs.get_components<ButtonComponent, SpriteComponent, ButtonHoveredEvent>()) {
        sprite.sprite = &SpriteManager::instance().get(button.hovered_color_id);
        AudioManager::instance().play("hover_button");
    }
    for (auto [button, sprite, _] : ecs.get_components<ButtonComponent, SpriteComponent, ButtonPressedEvent>()) {
        sprite.sprite = &SpriteManager::instance().get(button.pressed_color_id);
    }
}

}  // namespace core
