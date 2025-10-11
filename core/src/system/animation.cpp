#include <core/system/animation.hpp>
#include <core/component/sprite.hpp>
#include <core/component/animation.hpp>
#include <core/component/animation_fsm.hpp>
#include <core/component/direction.hpp>
#include <core/component/speed.hpp>
#include <core/manager/animation.hpp>
#include <core/manager/time.hpp>

#include <ecs/ecs.hpp>
#include <wheel/geometry.hpp>
#include <wheel/log.hpp>

namespace core {

void AnimationSystem::operator()(wheel::ECS& ecs) {
    // update animation fsm condition values
    for (auto [animation_fsm, speed, direction] : ecs.get_components<AnimationFSMComponent, SpeedComponent, DirectionComponent>()) {
        auto is_moving = speed.speed > 0.f && direction.move != 0.f;
        animation_fsm.next_condition2values["is_walking"] = is_moving;
        animation_fsm.next_condition2values["is_idle"] = !is_moving;
    }
    for (auto [animation, animation_fsm] : ecs.get_components<AnimationComponent, AnimationFSMComponent>()) {
        const auto& anim = *animation.animation;
        auto index = static_cast<size_t>(animation.time / anim.duration);
        if (!anim.loop && index >= anim.sprites.size() - 1) {
            animation_fsm.next_condition2values["is_animation_finished"] = true;
        }
    }

    // update animation state
    for (auto [animation_fsm, animation] : ecs.get_components<AnimationFSMComponent, AnimationComponent>()) {
        const auto& fsm = *animation_fsm.fsm;
        const auto& condition2values = animation_fsm.current_condition2values;
        auto& current_state = animation_fsm.current_state;
        if (current_state == "end") {
            continue;
        }
        if (!fsm.transitions.count(current_state)) {
            wheel::Log::warning("AnimationFSM has no transitions for state: {}", current_state);
            continue;
        }
        const auto& current_state_transitions = fsm.transitions.at(animation_fsm.current_state);
        for (const auto& [next_state, conditions] : current_state_transitions) {
            bool satisfied = true;
            for (const auto& condition : conditions) {
                if (auto it = condition2values.find(condition); it == condition2values.end() || !it->second) {
                    satisfied = false;
                    break;
                }
            }
            if (satisfied) {
                current_state = next_state;
                if (current_state != "start" && current_state != "end") {
                    animation.animation = &AnimationManager::instance().get({animation.animation->id.name, next_state});
                    if (!animation.animation->loop) {
                        animation.time = 0.f;
                    }
                }
                break;
            }
        }
    }

    // update animation time
    for (auto [animation] : ecs.get_components<AnimationComponent>()) {
        animation.time += static_cast<float>(TimeManager::instance().dt()) / wheel::Timer::TIME_UNIT_PER_SECOND;
    }

    // update sprite flip
    for (auto [sprite, direction] : ecs.get_components<SpriteComponent, DirectionComponent>()) {
        sprite.flip = direction.look[0] < 0;
    }

    // update animation sprite frame
    for (auto [sprite, animation]
        : ecs.get_components<SpriteComponent, AnimationComponent>()) {
        const auto& anim = *animation.animation;
        auto index = static_cast<size_t>(animation.time / anim.duration);
        if (anim.loop) {
            index %= anim.sprites.size();
        } else {
            index = std::min(index, anim.sprites.size() - 1);
        }
        sprite.sprite = &anim.sprites.at(index);
    }

    // swap and clear animation fsm condition values
    for (auto [animation_fsm] : ecs.get_components<AnimationFSMComponent>()) {
        animation_fsm.current_condition2values = std::move(animation_fsm.next_condition2values);
    }
}

}  // namespace core
