#include <game/layer/status.hpp>

#include <game/global.hpp>

#include <game/component/self.hpp>
#include <game/component/level.hpp>
#include <game/component/hp.hpp>
#include <game/component/inventory.hpp>

namespace wheel {

void StatusLayer::on_attach() {

}

void StatusLayer::on_detach() {

}

void StatusLayer::on_update() {

}

void StatusLayer::on_render() {
    if (!ecs.has_components<SelfComponent>()) {
        return;
    }

    int entity = ecs.get_entities<SelfComponent>()[0];
    auto& level = ecs.get_component<LevelComponent>(entity);
    auto& hp = ecs.get_component<HPComponent>(entity);
    std::string s = std::format("level: {}, exp: {}/{}, hp: {}/{}, kill: {}",
                                level.level,
                                level.exp,
                                config_resource.levelup_exps[level.level],
                                hp.hp,
                                hp.max_hp,
                                player_resource.killed);

    auto& inventory = ecs.get_component<InventoryComponent>(entity).inventory;
    auto& item = inventory.selected_slot().item();
    if (!item.empty()) {
        s += " " + item.info();
    }

    SDL_FRect dst_ui = {20, 20, 20 + (float)width, (float)height};
    sdl.draw_text(s, &dst_ui, sdl.WHITE);
}

bool StatusLayer::on_event(const SDL_Event& event) {
    return false;
}

}  // namespace wheel
