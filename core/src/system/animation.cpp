#include <core/system/animation.hpp>
#include <core/global.hpp>
#include <core/component/sprite.hpp>
#include <core/component/animation.hpp>
#include <core/component/direction.hpp>
#include <core/manager/animation.hpp>
#include <core/manager/time.hpp>

namespace core {

void AnimationSystem::update_impl() {
    auto& animation_manager = AnimationManager::instance();

    for (auto [animation] : ecs.get_components<AnimationComponent>()) {
        animation.time += static_cast<float>(TimeManager::instance().dt()) / wheel::Timer::TIME_UNIT_PER_SECOND;
    }

    for (auto [animation, direction] : ecs.get_components<AnimationComponent, DirectionComponent>()) {
        animation.animation = &animation_manager.get(
            animation.animation->name + "-" + direction_to_string_(direction.look)
        );
    }

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
}

std::string AnimationSystem::direction_to_string_(const wheel::Vector2D<float>& direction) {
    if (std::abs(direction[1]) >= std::abs(direction[0])) {
        if (direction[1] < 0) {
            return "up";
        } else {
            return "down";
        }
    } else {
        if (direction[0] < 0) {
            return "left";
        } else {
            return "right";
        }
    }
    return "down";
}

}  // namespace core
