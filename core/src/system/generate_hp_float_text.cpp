#include <core/system/generate_hp_float_text.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/sprite.hpp>
#include <core/component/transform.hpp>
#include <core/component/direction.hpp>
#include <core/component/speed.hpp>
#include <core/component/sprite.hpp>
#include <core/component/render.hpp>
#include <core/component/hp.hpp>
#include <core/tag/render.hpp>
#include <core/event/hp_change.hpp>
#include <core/entity_event/remove_entity.hpp>

#include <wheel/random.hpp>

#include <format>

namespace core {

void GenerateHPFloatTextSystem::operator()(wheel::ECS& ecs) {
    for (auto [source, target, value] : ecs.get_events<HPChangeEvent>()) {
        if (ecs.has_component<HPComponent>(target)) {
            // generate HP change float text
            const auto& target_transform = ecs.get_component<TransformComponent>(target);
            auto& sprite_manager = SpriteManager::instance();
            wheel::ID id = std::format("hp_float_text_{}", value);
            if (!sprite_manager.has(id)) {
                auto texture = sdl::SDL::create_texture(std::to_string(std::abs(value)), 20.f, value > 0 ? sdl::SDL::Color::Green : sdl::SDL::Color::Red);
                sprite_manager.set(id, Sprite{texture});
            }
            auto [w, h] = sdl::SDL::get_texture_size(sprite_manager.get(id).texture);
            auto position = target_transform.global.position;
            auto size = target_transform.global.size;
            auto text_entity = EntityManager::instance().add_entity(
                TransformComponent{
                    position - wheel::Vector2D<float>{0.f, size[1] / 2.f + 10.f},
                    {w, h}
                },
                DirectionComponent{{wheel::Random::instance().uniform<float>(-0.5f, 0.5f), -1.f}},
                SpeedComponent{wheel::Random::instance().uniform<float>(10.f, 20.f)},
                SpriteComponent{id},
                RenderComponent{1},
                RenderTag{}
            );
            timer.add(1000000, [text_entity, &ecs]() {
                if (ecs.has_entity(text_entity)) {
                    ecs.add_entity_event(text_entity, RemoveEntityEvent{});
                }
                return 0;
            });
        }
    }
}

}  // namespace core
