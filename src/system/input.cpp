// 1. TODO: serialize

#include <game/system/input.hpp>

#include <game/global.hpp>
#include <game/client.hpp>
#include <game/event.hpp>
#include <game/entity_manager.hpp>
#include <game/game_manager.hpp>

#include <game/component/input.hpp>
#include <game/component/actions.hpp>

namespace wheel {

void InputSystem::execute_impl() {
    auto& queue = Client::instance().get_queue();
    while (!queue.empty()) {
        auto msg = queue.front(); queue.pop();

        auto ret = Json::parse(msg);
        if (ret.empty()) {
            continue;
        }

        JsonDictType obj = ret;

        if (obj.count("entity")) {
            int entity = obj["entity"];
            std::string type = obj["type"];
            int keycode = obj["code"];

            auto& input = ecs.get_component<InputComponent>(entity);
            if (input.key_bindings.count(keycode)) {
                std::string action_name = input.key_bindings[keycode];
                auto& actions = ecs.get_component<ActionsComponent>(entity);
                if (type == "down") {
                    if (!GameManager::instance().paused()) {
                        actions.action_map[action_name]->start();
                    }
                } else if (type == "up") {
                    actions.action_map[action_name]->finish();
                }
            }
        }
    }
}

}  // namespace wheel
