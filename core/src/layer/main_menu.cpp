#include <core/layer/main_menu.hpp>
#include <core/global.hpp>
#include <core/manager/game.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/sprite.hpp>
#include <core/manager/ui.hpp>
#include <core/component/text.hpp>
#include <core/component/button.hpp>
#include <core/component/sprite.hpp>
#include <core/component/layer.hpp>
#include <core/tag/render.hpp>
#include <core/layer/game.hpp>

namespace core {

void MainMenuLayer::on_attach() {
    auto& game_manager = GameManager::instance();
    auto& entity_manager = EntityManager::instance();

    wheel::Entity button0_entity;
    switch (game_manager.state()) {
        case GameManager::State::MAIN_MENU: {
            button0_entity = entity_manager.add_entity(
                ButtonComponent{[]() {
                    UIManager::instance().pop_back();
                    UIManager::instance().push_back<GameLayer>();
                }},
                TransformComponent{
                    {context.virtual_window_width / 2.f, context.virtual_window_height / 2.f - 50.f},
                    {200.f, 80.f},
                    {1.f, 1.f},
                    Coordinate::Type::Screen
                },
                SpriteComponent{},
                LayerComponent{3},
                RenderTag{}
            );
            auto text0_entity = entity_manager.add_entity(
                button0_entity,
                TextComponent{"new", 32, sdl::SDL::BLACK},
                TransformComponent{},
                SpriteComponent{},
                LayerComponent{4},
                RenderTag{}
            );
            break;
        }
        case GameManager::State::PAUSED: {
            button0_entity = entity_manager.add_entity(
                ButtonComponent{[]() {
                    GameManager::instance().resume();
                }},
                TransformComponent{
                    {context.virtual_window_width / 2.f, context.virtual_window_height / 2.f - 50.f},
                    {200.f, 80.f},
                    {1.f, 1.f},
                    Coordinate::Type::Screen
                },
                SpriteComponent{},
                LayerComponent{3},
                RenderTag{}
            );
            auto text0_entity = entity_manager.add_entity(
                button0_entity,
                TextComponent{"resume", 32, sdl::SDL::BLACK},
                TransformComponent{},
                SpriteComponent{},
                LayerComponent{4},
                RenderTag{}
            );
            break;
        }
        case GameManager::State::RUNNING: {
            break;
        }
    }

    auto button1_entity = entity_manager.add_entity(
        ButtonComponent{[]() {
            context.running = false;
        }},
        TransformComponent{
            {context.virtual_window_width / 2.f, context.virtual_window_height / 2.f + 50.f},
            {200.f, 80.f},
            {1.f, 1.f},
            Coordinate::Type::Screen
        },
        SpriteComponent{},
        LayerComponent{3},
        RenderTag{}
    );
    auto text1_entity = entity_manager.add_entity(
        button1_entity,
        TextComponent{"exit", 32, sdl::SDL::BLACK},
        TransformComponent{},
        SpriteComponent{},
        LayerComponent{4},
        RenderTag{}
    );

    button_entities_ = {button0_entity, button1_entity};
    MenuLayer::on_attach();
}

void MainMenuLayer::on_detach() {
    if (GameManager::instance().state() == GameManager::State::PAUSED) {
        GameManager::instance().resume();
    }
    MenuLayer::on_detach();
}

}  // namespace core
