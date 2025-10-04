#include <core/layer/menu.hpp>
#include <core/global.hpp>
#include <core/manager/sprite.hpp>
#include <core/component/button.hpp>
#include <core/component/sprite.hpp>
#include <core/entity_event/remove_entity.hpp>

namespace core {

void MenuLayer::on_attach() {
    hover_button_(0);
}

void MenuLayer::on_detach() {
    for (auto& entity : button_entities_) {
        ecs.add_entity_event(entity, RemoveEntityEvent{});
    }
}

bool MenuLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_J: {
                    hover_button_(1);
                    return true;
                }
                case SDLK_K: {
                    hover_button_(-1);
                    return true;
                }
                case SDLK_RETURN: {
                    auto& button = ecs.get_component<ButtonComponent>(button_entities_[index_]);
                    button.state = ButtonComponent::State::CLICK;
                    auto& sprite = ecs.get_component<SpriteComponent>(button_entities_[index_]);
                    sprite.sprite = &SpriteManager::instance().get(button.click_color);
                    button.callback();
                    return true;
                }
            }
            break;
        }
        case SDL_EVENT_KEY_UP: {
            switch (event.key.key) {
                case SDLK_RETURN: {
                    auto& button = ecs.get_component<ButtonComponent>(button_entities_[index_]);
                    button.state = ButtonComponent::State::NORMAL;
                    auto& sprite = ecs.get_component<SpriteComponent>(button_entities_[index_]);
                    sprite.sprite = &SpriteManager::instance().get(button.normal_color);
                    button.callback();
                    return true;
                }
            }
        }
    }
    return false;
}

void MenuLayer::hover_button_(int index_delta) {
    auto& sprite_manager = SpriteManager::instance();

    size_t new_index = (index_ + index_delta + button_entities_.size()) % button_entities_.size();

    auto& old_button = ecs.get_component<ButtonComponent>(button_entities_[index_]);
    if (old_button.state == ButtonComponent::State::HOVER) {
        old_button.state = ButtonComponent::State::NORMAL;
        auto& old_sprite = ecs.get_component<SpriteComponent>(button_entities_[index_]);
        old_sprite.sprite = &sprite_manager.get(old_button.normal_color);
    }

    auto& new_button = ecs.get_component<ButtonComponent>(button_entities_[new_index]);
    if (new_button.state != ButtonComponent::State::HOVER) {
        new_button.state = ButtonComponent::State::HOVER;
        auto& new_sprite = ecs.get_component<SpriteComponent>(button_entities_[new_index]);
        new_sprite.sprite = &sprite_manager.get(new_button.hover_color);
    }

    index_ = new_index;
}

}  // namespace core
