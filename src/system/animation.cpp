#include <game/system/animation.hpp>

#include <game/global.hpp>
#include <game/animation_manager.hpp>
#include <game/utils.hpp>

#include <game/component/direction.hpp>
#include <game/component/animation.hpp>
#include <game/component/texture.hpp>

namespace wheel {

void AnimationSystem::execute_impl() {
    update_animation_counter();
    update_texture();
}

void AnimationSystem::update_animation_counter() {
    for (auto [animation] : ecs.get_components<AnimationComponent>()) {
        if (animation.type == Animations::Type::ATTACK) {
            continue;
        }

        // TODO: not only
        if (animation.counter < 30) {
            animation.counter++;
            continue;
        }
        animation.counter = 0;

        // TODO: not cache friendly
        animation.idx = (animation.idx + 1) % animation.animations->frames();
    }
}

void AnimationSystem::update_texture() {
    for (auto [entity, animation, texture] : ecs.get_entity_and_components<AnimationComponent, TextureComponent>()) {
        Vector2D<float> direction = {0, 1};
        if (ecs.has_components<DirectionComponent>(entity)) {
            direction = ecs.get_component<DirectionComponent>(entity).vec;
        }
        texture.texture = animation.animations->get_texture(animation.idx, GameUtils::direction2orientation(direction), animation.type);
    }
}

}  // namespace wheel
