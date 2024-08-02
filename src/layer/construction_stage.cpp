#include <game/layer/construction_stage.hpp>

#include <game/global.hpp>
#include <game/game_manager.hpp>

namespace wheel {

ConstructionStageLayer::ConstructionStageLayer()
    : start_button_("start", 20, []() { GameManager::instance().swap_stage(); }) {
    // start button at right top
    int window_padding = 20;
    auto [w, h] = start_button_.size();
    start_button_.dst() = {
        (float)config_resource.w - w - window_padding,
        (float)window_padding,
        (float)w,
        (float)h
    };
}

void ConstructionStageLayer::on_attach() {
}

void ConstructionStageLayer::on_detach() {
}

void ConstructionStageLayer::on_update() {
}

void ConstructionStageLayer::on_render() {
    start_button_.on_render();
}

bool ConstructionStageLayer::on_event(const SDL_Event& event) {
    switch (event.type) {
        case SDL_EVENT_KEY_DOWN: {
            switch (event.key.key) {
                case SDLK_RETURN: {
                    GameManager::instance().swap_stage();
                    return true;
                }
            }
        }
    }
    return start_button_.on_event(event);
}

}  // namespace wheel
