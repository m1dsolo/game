#include <core/util/coordinate.hpp>
#include <core/global.hpp>
#include <core/component/transform.hpp>
#include <core/tag/camera.hpp>
#include <core/resource/context.hpp>

namespace core {

SDL_FRect Coordinate::world2screen(const SDL_FRect& world_rect) {
    auto camera = ecs.get_entity<CameraTag>();
    auto camera_rect = ecs.get_component<TransformComponent>(camera).global.rect();

    const auto& context = ecs.get_resource<ContextResource>();
    return {
        (world_rect.x - camera_rect.x) / camera_rect.w * context.virtual_window_width,
        (world_rect.y - camera_rect.y) / camera_rect.h * context.virtual_window_height,
        world_rect.w / camera_rect.w * context.virtual_window_width,
        world_rect.h / camera_rect.h * context.virtual_window_height,
    };
}

}  // namespace core
