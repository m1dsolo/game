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
    switch_animation_state();
}

void AnimationSystem::update_animation_counter() {
    for (auto [animation] : ecs.get_components<AnimationComponent>()) {
        // TODO: not only
        if (animation.counter < animation.max_counter) {
            animation.counter++;
            continue;
        }
        animation.counter = 0;
        animation.idx = (animation.idx + 1) % animation.frames;
    }
}

void AnimationSystem::update_texture() {
    for (auto [entity, animation, texture] : ecs.get_entity_and_components<AnimationComponent, TextureComponent>()) {
        Vector2D<float> direction = {0, 1};
        if (ecs.has_components<DirectionComponent>(entity)) {
            direction = ecs.get_component<DirectionComponent>(entity).vec;
        }
        texture.texture = animation.animations->get_texture(animation.idx, GameUtils::direction2orientation(direction), animation.state);
    }
}

// TODO: FSM
void AnimationSystem::switch_animation_state() {
    for (auto [entity, animation, switch_state] : ecs.get_entity_and_components<AnimationComponent, SwitchAnimationStateComponent>()) {
        if (animation.state == switch_state.new_state) {
            continue;
        }
        if (switch_state.duration != -1) {
            timer_resource.add(switch_state.duration, 1, [&, entity, state = animation.state, max_counter = animation.max_counter]() {
                if (ecs.has_components<SwitchAnimationStateComponent>(entity)) {
                    ecs.del_components<SwitchAnimationStateComponent>(entity);
                }
                ecs.add_components(entity, SwitchAnimationStateComponent{state, max_counter});
            });
        }

        animation.state = switch_state.new_state;
        if (switch_state.max_counter != -1) {
            animation.max_counter = switch_state.max_counter;
        }

        if (switch_state.new_state == Animations::State::ATTACK) {
            animation.counter = 0;
            animation.idx = 0;
        }
    }
    ecs.del_components<SwitchAnimationStateComponent>();
}

}  // namespace wheel
