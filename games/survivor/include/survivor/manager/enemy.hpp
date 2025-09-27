#pragma once

#include <survivor/config/enemy.hpp>

#include <wheel/singleton.hpp>
#include <ecs/entity.hpp>

#include <string>
#include <vector>
#include <unordered_map>

namespace survivor {

class EnemyManager : public wheel::Singleton<EnemyManager> {
    friend wheel::Singleton<EnemyManager>;

public:
    void generate_waves() const;
    wheel::Entity generate(const std::string& name) const;

private:
    EnemyManager();
    EnemyManager(const EnemyManager&) = delete;

    std::vector<wheel::Entity> boundaries_;
    std::unordered_map<std::string, EnemyConfig> enemy_configs_;
};

}  // namespace survivor
