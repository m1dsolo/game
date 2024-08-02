#include <game/item/item.hpp>

#include <game/audio_manager.hpp>

#include <game/component/actions.hpp>
#include <game/component/input.hpp>

namespace wheel {

void Item::select() {
    if (ecs.has_components<ActionsComponent>(entity_)) {
        auto& action_map = ecs.get_component<ActionsComponent>(entity_).action_map;
        for (auto& [name, action] : action_map_) {
            action_map[name] = action;
        }
    }
    if (ecs.has_components<InputComponent>(entity_)) {
        auto& key_bindings = ecs.get_component<InputComponent>(entity_).key_bindings;
        for (auto& [name, action] : action_map_) {
            key_bindings[action->keycode()] = action->name();
        }
    }

    select_impl();

    AudioManager::instance().play(data_->name + "_select");
}

void Item::unselect() {
    if (ecs.has_components<ActionsComponent>(entity_)) {
        auto& action_map = ecs.get_component<ActionsComponent>(entity_).action_map;
        for (auto& [name, action] : action_map_) {
            action_map.erase(name);
        }
    }
    if (ecs.has_components<InputComponent>(entity_)) {
        auto& key_bindings = ecs.get_component<InputComponent>(entity_).key_bindings;
        for (auto& [name, action] : action_map_) {
            key_bindings.erase(action->keycode());
        }
    }

    unselect_impl();
}

}  // namespace wheel
