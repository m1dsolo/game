#include <core/system/animation.hpp>
#include <core/global.hpp>
#include <core/component/sprite.hpp>
#include <core/component/animation.hpp>
#include <core/component/animation_fsm.hpp>
#include <core/component/direction.hpp>
#include <core/component/speed.hpp>
#include <core/manager/animation.hpp>
#include <core/manager/time.hpp>

#include <wheel/geometry.hpp>

namespace core {

void AnimationSystem::operator()() {
    // update animation fsm condition values
    for (auto [animation_fsm, speed, direction] : ecs.get_components<AnimationFSMComponent, SpeedComponent, DirectionComponent>()) {
        auto is_moving = speed.speed > 0.f && direction.move != 0.f;
        animation_fsm.condition2values["is_moving"] = is_moving;
        animation_fsm.condition2values["is_idle"] = !is_moving;
    }

    // update animation state
    for (auto [animation_fsm, animation] : ecs.get_components<AnimationFSMComponent, AnimationComponent>()) {
        const auto& fsm = *animation_fsm.fsm;
        const auto& condition2values = animation_fsm.condition2values;
        auto& current_state = animation_fsm.current_state;
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
                animation.animation = &AnimationManager::instance().get({animation.animation->id.name, next_state});
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
        const auto& clip = *animation.animation;
        auto index = static_cast<size_t>(animation.time / clip.duration);
        if (clip.loop) {
            index %= clip.sprites.size();
        } else {
            index = std::min(index, clip.sprites.size() - 1);
        }
        sprite.sprite = &clip.sprites.at(index);
    }
}

}  // namespace core
