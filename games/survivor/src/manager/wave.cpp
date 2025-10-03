#include <survivor/manager/wave.hpp>
#include <core/global.hpp>
#include <core/manager/time.hpp>
#include <core/manager/enemy.hpp>
#include <core/component/name.hpp>
#include <core/component/transform.hpp>

#include <wheel/geometry.hpp>
#include <wheel/random.hpp>

using namespace core;

namespace survivor {

wheel::Vector2D<float> random_pick_point(const wheel::Rect<float>& rect);

WaveManager::WaveManager() {
    for (const auto& [entity, name] : ecs.get_entity_and_components<NameComponent>()) {
        if (name.name.find("boundary") != std::string::npos) {
            boundaries_.emplace_back(entity);
        }
    }
}

void WaveManager::generate_waves() const {
    TimeManager::instance().timer().add(1000000, [this]() {
        generate("thief");

        return 1000000;
    });
}

wheel::Entity WaveManager::generate(const std::string& name) const {
    auto boundary = boundaries_[wheel::Random::instance().uniform(0, static_cast<int>(boundaries_.size()) - 1)];
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
