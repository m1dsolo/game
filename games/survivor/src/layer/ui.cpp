#include <survivor/layer/ui.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/texture.hpp>
#include <core/component/text.hpp>
#include <core/component/transform.hpp>
#include <core/component/sprite.hpp>
#include <core/component/render.hpp>
#include <core/tag/input.hpp>
#include <survivor/manager/achievement.hpp>
#include <survivor/component/hp.hpp>

#include <sdl/sdl.hpp>

#include <format>

using namespace core;

namespace survivor {

void UILayer::on_attach() {
    player_entity_ = ecs.get_entity<InputTag>();
    text_entity_ = EntityManager::instance().add_entity(
        TextComponent{"", 32, sdl::SDL::ORANGE},
        TransformComponent{
            {0.1f * config.virtual_window_width, 0.1f * config.virtual_window_height},
            {0.f, 0.f},
            {1.f, 1.f},
            Coordinate::Type::SCREEN
        },
        SpriteComponent{},
        RenderComponent{3}
    );
}

void UILayer::on_render() {
    if (!ecs.has_component<HPComponent>(player_entity_)) {
        return;
    }

    const auto& achievement_manager = AchievementManager::instance();
    const auto& hp = ecs.get_component<HPComponent>(player_entity_);
    auto text = std::format("hp:{}/{} kill:{}", hp.hp, hp.max_hp, achievement_manager.kill_num());
    EntityManager::instance().update_text(text_entity_, text);
}

bool UILayer::on_event(const SDL_Event& event) {
    return false;
}

}  // namespace survivor
