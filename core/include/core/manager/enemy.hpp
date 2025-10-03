#pragma once

#include <core/config/enemy.hpp>

#include <wheel/singleton.hpp>
#include <wheel/geometry.hpp>
#include <ecs/entity.hpp>

#include <string>
#include <unordered_map>

namespace core {

class EnemyManager : public wheel::Singleton<EnemyManager> {
    friend wheel::Singleton<EnemyManager>;

public:
    wheel::Entity generate(
        const std::string& name,
        wheel::Vector2D<float> position = {0.f, 0.f}
    ) const;

private:
    EnemyManager();
    EnemyManager(const EnemyManager&) = delete;

    std::unordered_map<std::string, EnemyConfig> enemy_configs_;
};

}  // namespace core
