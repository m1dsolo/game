#include <core/system/generate_hp_float_text.hpp>
#include <core/global.hpp>
#include <core/manager/entity.hpp>
#include <core/manager/sprite.hpp>
#include <core/manager/time.hpp>
#include <core/component/transform.hpp>
#include <core/component/direction.hpp>
#include <core/component/speed.hpp>
#include <core/component/sprite.hpp>
#include <core/component/render.hpp>
#include <core/component/hp.hpp>
#include <core/event/hp_change.hpp>
#include <core/entity_event/remove_entity.hpp>

#include <wheel/random.hpp>

namespace core {

void GenerateHPFloatTextSystem::operator()() {
    for (auto [source, target, value] : ecs.get_events<HPChangeEvent>()) {
        if (ecs.has_component<HPComponent>(target)) {
            // generate HP change float text
            const auto& target_transform = ecs.get_component<TransformComponent>(target);
            auto& sprite_manager = SpriteManager::instance();
            auto key = "hp_change" + std::to_string(value);
            if (!sprite_manager.has(key)) {
                auto texture = sdl::SDL::create_texture(std::to_string(std::abs(value)), 20.f, value > 0 ? sdl::SDL::GREEN : sdl::SDL::RED);
                sprite_manager.set(key, Sprite{texture});
            }
            auto [w, h] = sdl::SDL::get_texture_size(sprite_manager.get(key).texture);
            auto position = target_transform.global.position;
            auto size = target_transform.global.size;
            auto text_entity = EntityManager::instance().add_entity(
                TransformComponent{
                    position - wheel::Vector2D<float>{0.f, size[1] / 2.f + 10.f},
                    {w, h}
                },
                DirectionComponent{{wheel::Random::instance().uniform<float>(-0.5f, 0.5f), -1.f}},
                SpeedComponent{wheel::Random::instance().uniform<float>(10.f, 20.f)},
                SpriteComponent{key},
                RenderComponent{1}
            );
            TimeManager::instance().timer().add(1000000, [text_entity]() {
                if (ecs.has_entity(text_entity)) {
                    ecs.add_entity_event(text_entity, RemoveEntityEvent{});
                }
                return 0;
            });
        }
    }
}

}  // namespace core
