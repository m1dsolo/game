#include <survivor/manager/wave.hpp>
#include <core/global.hpp>
#include <core/manager/enemy.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>

#include <wheel/geometry.hpp>
#include <wheel/random.hpp>

using namespace core;

namespace survivor {

wheel::Vector2D<float> random_pick_point(const wheel::Rect<float>& rect);

void WaveManager::start_generate_waves() {
    std::vector<wheel::Entity> boundaries;
    for (const auto& [entity, name] : ecs.get_entity_and_components<NameComponent>()) {
        if (name.name.find("boundary") != std::string::npos) {
            boundaries.emplace_back(entity);
        }
    }

    timer_id_ = timer.add(1000000, [this, boundaries = std::move(boundaries)]() {
        generate("thief", boundaries);

        return 1000000;
    });
}

void WaveManager::stop_generate_waves() {
    timer.remove(timer_id_);
}

wheel::Entity WaveManager::generate(const std::string& name, const std::vector<wheel::Entity>& boundaries) const {
    auto boundary = boundaries[wheel::Random::instance().uniform(0, static_cast<int>(boundaries.size()) - 1)];
    const auto& transform = ecs.get_component<TransformComponent>(boundary).global;
    auto pos = random_pick_point(wheel::Rect<float>{transform.position, transform.size});
    return EnemyManager::instance().generate(name, pos);
}

wheel::Vector2D<float> random_pick_point(const wheel::Rect<float>& rect) {
    auto& random = wheel::Random::instance();
    return {
        random.uniform(rect.x0, rect.x1),
        random.uniform(rect.y0, rect.y1)
    };
}

}  // namespace survivor
