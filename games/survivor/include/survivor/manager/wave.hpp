#pragma once

#include <ecs/entity.hpp>
#include <wheel/singleton.hpp>

#include <string>
#include <vector>

namespace survivor {

class WaveManager : public wheel::Singleton<WaveManager> {
    friend wheel::Singleton<WaveManager>;

public:
    void generate_waves() const;
    wheel::Entity generate(const std::string& name) const;

private:
    WaveManager();
    WaveManager(const WaveManager&) = delete;

    std::vector<wheel::Entity> boundaries_;
};

}  // namespace survivor
